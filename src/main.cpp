//
// Created by sliberman on 3/26/19.
//

#include <iostream>
#include "mpi.h"
#include "GeneticAlgorithm.h"
#include "Problem.h"
#include "Point.h"
#include "Individual.h"
#include <unistd.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void kill_workers() {
    int end_flag = 1;
    MPI_Bcast(&end_flag, 1, MPI_INT, 0, MPI_COMM_WORLD);
}

int check_break() {
    int flag = -1;
    MPI_Bcast(&flag, 1, MPI_INT, 0, MPI_COMM_WORLD);
    return flag;
}

void process_draw(int rank, unsigned char *buf_a, int *buf_ind, int width, int height, int P) {
    int offset, new_height;
    new_height = height / P;
    offset = new_height * rank;

    if (rank == P - 1) {
        new_height += height % P;
    }

    int len_each = height / P * width * 4;
    int residual = height * width * 4 - len_each * P;

    cv::Mat img = cv::Mat(new_height, width, CV_8UC4, buf_a);

    // cout << "rank " << rank << ": receiving scattered canvas" << endl;
    //cout << "1:" << rank << " receiving " << small << endl;
    MPI_Scatter(nullptr, len_each, MPI_UNSIGNED_CHAR,
                buf_a, len_each, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    //cout << "1:" << rank << " received " << small << endl;
    if (rank == P - 1) {
        MPI_Status status;
        //cout << "2:" << rank << " receiving " << len_each - small << endl;
        MPI_Recv(buf_a + len_each, residual, MPI_UNSIGNED_CHAR, 0, 123, MPI_COMM_WORLD, &status);
        //cout << "2:" << rank << " received " << len_each - small << endl;
    }
    // cout << "rank " << rank << ": received scattered canvas" << endl;

    int sz_buf_ind;

    // cout << "rank " << rank << ": receiving buf_ind length" << endl;
    MPI_Bcast(&sz_buf_ind, 1, MPI_INT, 0, MPI_COMM_WORLD);
    // cout << "rank " << rank << ": buf_ind length is " << off << endl;
    // cout << "rank " << rank << ": receiving ind" << endl;
    MPI_Bcast(buf_ind, sz_buf_ind, MPI_INT, 0, MPI_COMM_WORLD);
    // cout << "rank " << rank << ": received ind" << endl;

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
        // cout << "rank " << rank << ": n_pts: " << *n_pts << endl;
        const cv::Point *ppt[1] = {pts};
        cv::fillPoly(partial_img, ppt, n_pts, 1, color_s, cv::LINE_8);

        cv::addWeighted(partial_img, ((double) a) / 255.0, img, 1.0 - ((double) a) / 255.0, 0.0, img);

        delete[] pts;
    }

    //cv::imwrite("part_" + to_string(rank) + ".bmp", img);

    // cout << "rank " << rank << ": gathering buffer" << endl;
    //cout << rank << ":3 sending " << small << endl;
    MPI_Gather(buf_a, len_each, MPI_UNSIGNED_CHAR, nullptr, len_each, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    //cout << "3:" << rank << " sent " << small << endl;
    if (rank == P - 1) {
        //cout << "4:" << rank << " sending " << len_each - small << endl;
        MPI_Send(buf_a + len_each, residual, MPI_UNSIGNED_CHAR, 0, 123, MPI_COMM_WORLD);
        //cout << "4:" << rank << " sent " << len_each - small << endl;
    }
    // cout << "rank " << rank << ": gathered buffer" << endl;
}

void process_diff(unsigned char *buf_a, unsigned char *buf_b, int len_each, int width, int height, int rank, int P) {
    // Process 2: diff
    int got_full = len_each * P;
    int expected_full = width * height * 4;
    int residual = expected_full - got_full;

    MPI_Status status;
    //          //            cout << "rank " << rank << ": pegado en a?" << endl;

    MPI_Scatter(nullptr, len_each, MPI_UNSIGNED_CHAR, buf_a, len_each, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    if (rank == P - 1) {
        MPI_Recv(buf_a + len_each, residual, MPI_UNSIGNED_CHAR, 0, 123, MPI_COMM_WORLD, &status);
    }
    //            cout << "rank " << rank << ": no pegado en a" << endl;
    //            cout << "rank " << rank << ": pegado en b?" << endl;
//    MPI_Scatter(nullptr, len_each, MPI_UNSIGNED_CHAR, buf_b, len_each, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
//    if (rank == P - 1) {
//        MPI_Recv(buf_b + len_each, residual, MPI_UNSIGNED_CHAR, 0, 123, MPI_COMM_WORLD, &status);
//    }
    //            cout << "rank " << rank << ": no pegado en b" << endl;

//    auto img1 = cv::Mat(height / P, width, CV_8UC4, buf_a);
//    cv::imwrite("im_a_" + to_string(rank) + ".bmp", img1);
//
//    auto img2 = cv::Mat(height / P, width, CV_8UC4, buf_b);
//    cv::imwrite("im_b_" + to_string(rank) + ".bmp", img2);

    int last = len_each;
    if (rank == P - 1) {
        last += residual;
    }

    unsigned char *buf_b_piece_start = buf_b + rank * len_each;

    int e1 = 0;
    for (int i = 0; i < last; i++) {
        e1 += abs(buf_a[i] - buf_b_piece_start[i]);
    }
    //            cout << "rank " << rank << ": " << e1 << endl;

    MPI_Reduce(&e1, nullptr, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
}

int main(int argc, char **argv) {
    //    if (rank == 0) {
    /* Rank 0 should load the image */
    //        if (argc < 3) {
    //            cerr << "Not enough number of arguments" << endl;
    //            exit(1);
    //        }

    String file;
    int max_epochs = 1000;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-i") == 0) {
            String tmp(argv[i + 1]);
            file = tmp;
        }
        if (strcmp(argv[i], "-e") == 0) {
            max_epochs = stoi(argv[i + 1]);
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

    Mat image = imread(file, IMREAD_UNCHANGED);

    if (!mpi || rank == 0) {

        if (image.data) {
            Problem::run(&image, max_epochs, mpi);
        } else {
            cout << "Image not found. Exiting." << endl;
        }

        if (mpi) {
            kill_workers();
        }
    } else {
        // Process diff
        // Receive len
        auto buf_a = new unsigned char[10000000];
        auto buf_b = new unsigned char[10000000];
        auto buf_ind = new int[100000];

        int dims[2];
        // Get image dimensions
        // cout << "rank " << rank << ": waiting for dims" << endl;
        MPI_Bcast(&dims, 2, MPI_INT, 0, MPI_COMM_WORLD);
        cout << "rank " << rank << ": dims are " << dims[0] << "x" << dims[1] << endl;
        int width = dims[0];
        int height = dims[1];
        int len_each = height / P * width * 4;

        while (true) {
            if (check_break()) {
                break;
            }
            process_draw(rank, buf_a, buf_ind, width, height, P);

            if (check_break()) {
                break;
            }
            process_diff(buf_a, image.data, len_each, width, height, rank, P);
        }
        delete[] buf_a;
        delete[] buf_b;
        delete[] buf_ind;
    }
    if (mpi) {
        MPI_Finalize();
    }

    return 0;
}
