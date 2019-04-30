//
// Created by sliberman on 3/26/19.
//

#include <iostream>
#include "mpi.h"
#include "GeneticAlgorithm.h"
#include "Problem.h"
#include "Point.h"
#include "Individual.h"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

//const int TAG = 123;


int main(int argc, char **argv) {
    int rank, P;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);

//    if (rank == 0) {
        /* Rank 0 should load the image */
//        if (argc < 3) {
//            cerr << "Not enough number of arguments" << endl;
//            exit(1);
//        }
    int end_flag = 0;
    if (rank == 0) {
        String file;
        int max_epochs = 1000;
        for (int i = 1; i < argc; ++i) {
            if (strcmp(argv[i], "-i") == 0) {
                file = argv[i + 1];
            }
            if (strcmp(argv[i], "-e") == 0) {
                max_epochs = stoi(argv[i + 1]);
            }
            i++;
        }

        Mat image = imread(file, IMREAD_UNCHANGED);

        if(image.data )                              // Check for invalid input
        {
            /* Problem definition */
            /* 1) */
            Problem::run(&image, max_epochs);
        } else {
            cout << "Image not found. Exiting." << endl;
        }
        end_flag = 1;
        MPI_Bcast(&end_flag, 1, MPI_INT, 0, MPI_COMM_WORLD);
    } else {
        // Process diff
        // Receive len
        auto buf_a = new unsigned char[10000000];
        auto buf_b = new unsigned char[10000000];
        while (true) {
            cout << "rank " << rank << ": waiting for flag" << endl;
            MPI_Bcast(&end_flag, 1, MPI_INT, 0, MPI_COMM_WORLD);
            cout << "rank " << rank << ": flag value is " << end_flag << endl;

            if (end_flag) {
                cout << "rank " << rank << ": finished! with " << end_flag << endl;
                break;
            }

            int dims[2];
            cout << "rank " << rank << ": waiting for dims" << endl;
            MPI_Bcast(&dims, 2, MPI_INT, 0, MPI_COMM_WORLD);
            cout << "rank " << rank << ": dims are " << dims[0] << "x" << dims[1] << endl;

            int width = dims[0];
            int height = dims[1];
            int len_each = width * height * 4 / P;
            MPI_Scatter(nullptr, len_each, MPI_UNSIGNED_CHAR, buf_a, len_each, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

            int offset = height / P * rank;
            int new_height = height / P;

            for (int j = 0; j < len_each; ++j) {
                buf_a[j] = 0;
            }
            cv::Mat img = cv::Mat(new_height, width, CV_8UC4, buf_a);

            unsigned char buf_ind[10000];
            int off = 0;

            cout << "rank " << rank << ": receiving ind" << endl;
            MPI_Bcast(&off, 1, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Bcast(buf_ind, off, MPI_INT, 0, MPI_COMM_WORLD);
            cout << "rank " << rank << ": received ind" << endl;

            int r, g, b, a, x, y;
            int cur_off = 0;
            while (cur_off < off - 1) {
                cv::Mat partial_img = img.clone();
                int n_pts[] = {buf_ind[cur_off++]};
                r = buf_ind[cur_off++];
                g = buf_ind[cur_off++];
                b = buf_ind[cur_off++];
                a = buf_ind[cur_off++];
                cv::Scalar color_s = cv::Scalar(r, g, b);
                auto pts = new cv::Point[*n_pts];
                for (int i = 0; i < *n_pts; ++i) {
                    x = buf_ind[cur_off++];
                    y = buf_ind[cur_off++];
                    pts[i] = cv::Point(x, y + offset);
                }
                cout << "rank " << rank << ": n_pts: " << *n_pts << endl;
                const cv::Point* ppt[1] = {pts};
                cv::fillPoly(partial_img, ppt, n_pts, 1, color_s, cv::LINE_AA);

                cv::addWeighted(partial_img, ((double) a) / 255.0, img, 1.0 - ((double) a) / 255.0, 0.0, img);

                delete[] pts;

            }
            MPI_Gather(buf_a, len_each, MPI_UNSIGNED_CHAR, nullptr, len_each, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

//            cout << "rank " << rank << ": waiting for flag" << endl;
            MPI_Bcast(&end_flag, 1, MPI_INT, 0, MPI_COMM_WORLD);
//            cout << "rank " << rank << ": flag value is " << end_flag << endl;

            if (end_flag) {
//                cout << "rank " << rank << ": finished! with " << end_flag << endl;
                break;
            }

            int e1 = 0;

            //          //            cout << "rank " << rank << ": pegado en a?" << endl;
            MPI_Scatter(nullptr, len_each, MPI_UNSIGNED_CHAR, buf_a, len_each, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
//            cout << "rank " << rank << ": no pegado en a" << endl;
//            cout << "rank " << rank << ": pegado en b?" << endl;
            MPI_Scatter(nullptr, len_each, MPI_UNSIGNED_CHAR, buf_b, len_each, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
//            cout << "rank " << rank << ": no pegado en b" << endl;

            for (int i = 0; i < len_each; i++) {
                e1 += abs(buf_a[i] - buf_b[i]);
            }
//            cout << "rank " << rank << ": " << e1 << endl;

            MPI_Reduce(&e1, &e1, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        }
        delete[] buf_a;
        delete[] buf_b;
    }

    MPI_Finalize();

    return 0;
}