//
// Created by liberman and ayin on 3/26/19.
//

#include <iostream>
#include "mpi.h"
#include "GeneticAlgorithm.h"
#include "Problem.h"
#include "Point.h"
#include "Individual.h"
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <ctime>
#include "utils.h"

using namespace std;

void kill_workers() {
    int end_flag = 1;
    MPI_Bcast(&end_flag, 1, MPI_INT, 0, MPI_COMM_WORLD);
}

int check_break() {
    int flag = -1;
    MPI_Bcast(&flag, 1, MPI_INT, 0, MPI_COMM_WORLD);
    return flag;
}

void process_draw(int rank, unsigned char *buf_a, int *buf_ind, int width, int height, int channels, int P) {
    MPI_Request r1, r2;

    int sz_buf_ind;

    MPI_Ibcast(&sz_buf_ind, 1, MPI_INT, 0, MPI_COMM_WORLD, &r1);

    int offset, new_height;
    new_height = height / P;
    offset = new_height * rank;

    if (rank == P - 1) {
        new_height += height % P;
    }

    int len_each = height / P * width * channels;
    int residual = height * width * channels - len_each * P;

    cv::Mat img;
    if (channels == 4) {
        img = cv::Mat(new_height, width, CV_8UC4, buf_a);
    } else {
        img = cv::Mat(new_height, width, CV_8UC3, buf_a);
    }

    for (int i = 0; i < len_each + (rank == P - 1 ? residual : 0); i++) {
        buf_a[i] = 0;
    }

    MPI_Status s;
    MPI_Wait(&r1, &s);

    MPI_Ibcast(buf_ind, sz_buf_ind, MPI_INT, 0, MPI_COMM_WORLD, &r2);

    MPI_Wait(&r2, &s);


    int r, g, b, a, x, y;
    int cur_off = 0;
    while (cur_off < sz_buf_ind) {
        cv::Mat partial_img = img.clone();
        int n_pts[] = {buf_ind[cur_off++]};
        r = buf_ind[cur_off++];
        g = buf_ind[cur_off++];
        b = buf_ind[cur_off++];
        a = buf_ind[cur_off++];
        cv::Scalar color_s = cv::Scalar(b, g, r);
        auto pts = new cv::Point[*n_pts];
        for (int i = 0; i < *n_pts; ++i) {
            x = buf_ind[cur_off++];
            y = buf_ind[cur_off++];
            pts[i] = cv::Point(x, y - offset);
        }
        const cv::Point *ppt[1] = {pts};
        cv::fillPoly(partial_img, ppt, n_pts, 1, color_s, cv::LINE_8);

        cv::addWeighted(partial_img, ((double) a) / 255.0, img, 1.0 - ((double) a) / 255.0, 0.0, img);

        delete[] pts;
    }

    MPI_Igather(buf_a, len_each, MPI_UNSIGNED_CHAR, nullptr, len_each, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD, &r1);
    if (rank == P - 1) {
        MPI_Isend(buf_a + len_each, residual, MPI_UNSIGNED_CHAR, 0, 123, MPI_COMM_WORLD, &r2);
        MPI_Wait(&r2, &s);
    }
    MPI_Wait(&r1, &s);
}

void
process_diff(unsigned char *buf_a, unsigned char *buf_b, int len_each, int width, int height, int channels, int rank,
             int P) {
    int got_full = len_each * P;
    int expected_full = width * height * channels;
    int residual = expected_full - got_full;

    MPI_Status status;

    MPI_Scatter(nullptr, len_each, MPI_UNSIGNED_CHAR, buf_a, len_each, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    if (rank == P - 1) {
        MPI_Recv(buf_a + len_each, residual, MPI_UNSIGNED_CHAR, 0, 123, MPI_COMM_WORLD, &status);
    }

    int last = len_each;
    if (rank == P - 1) {
        last += residual;
    }

    unsigned char *buf_b_piece_start = buf_b + rank * len_each;

    int e1 = 0;
    for (int i = 0; i < last; i++) {
        e1 += abs(buf_a[i] - buf_b_piece_start[i]);
    }

    MPI_Reduce(&e1, nullptr, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
}

int main(int argc, char **argv) {
    clock_t start, end;
    double cpu_time_used;

    start = clock();

    string file, output_filename = "output/output.jpg";
    int max_epochs = 1000;
    utils::set_seed(time(0));
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-i") == 0) {
            string tmp(argv[i + 1]);
            file = tmp;
        }
        if (strcmp(argv[i], "-e") == 0) {
            max_epochs = stoi(argv[i + 1]);
        }
        if (strcmp(argv[i], "-o") == 0) {
            output_filename = argv[i + 1];
        }
        if (strcmp(argv[i], "-s") == 0) {
            utils::set_seed(stoi(argv[i + 1]));
        }
        i++;
    }
    bool mpi = false;

    int rank, P;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);

    if (P > 1) {
        if (rank == 0) {
            cout << "Using MPI!" << endl;
        }
        mpi = true;
    }

    cv::Mat image = cv::imread(file, cv::IMREAD_UNCHANGED);
    int channels = image.channels();


    if (!mpi || rank == 0) {

        if (image.data) {
            Problem::run(&image, max_epochs, mpi, channels, output_filename);
        } else {
            cout << "Image not found. Exiting." << endl;
        }

        if (mpi) {
            kill_workers();
        }
    } else {
        cv::Size s = image.size();
        int width = s.width;
        int height = s.height;
        int len_each = height / P * width * channels;
        int residual = height * width * channels - len_each * P;
        auto buf_a = new unsigned char[len_each + residual];
        auto buf_ind = new int[100000];


        while (true) {
            if (check_break()) {
                break;
            }
            process_draw(rank, buf_a, buf_ind, width, height, channels, P);

            if (check_break()) {
                break;
            }
            process_diff(buf_a, image.data, len_each, width, height, channels, rank, P);
        }
        delete[] buf_a;
        delete[] buf_ind;
    }
    if (mpi) {
        MPI_Finalize();
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    if (rank == 0) {
        cout << "time spent: " << cpu_time_used << endl;
    }

    return 0;
}
