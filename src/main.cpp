//
// Created by sliberman on 3/26/19.
//

#include <iostream>
#include "mpi.h"
#include "GeneticAlgorithm.h"
#include "Problem.h"
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
            end_flag = 1;
            MPI_Bcast(&end_flag, 1, MPI_INT, 0, MPI_COMM_WORLD);
        }
    } else {
        // Process diff
        // Receive len
        auto buf_a = new unsigned char[10000000];
        auto buf_b = new unsigned char[10000000];
        while (end_flag == 0) {
            int len = 0, e1 = 0;
            MPI_Bcast(&len, 1, MPI_INT, 0, MPI_COMM_WORLD);
            int len_each = len / P;

            MPI_Scatter(nullptr, len_each, MPI_UNSIGNED_CHAR, buf_a, len_each, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
            MPI_Scatter(nullptr, len_each, MPI_UNSIGNED_CHAR, buf_b, len_each, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

            for (int i = 0; i < len_each; i++) {
                e1 += abs(buf_a[i] - buf_b[i]);
            }
            MPI_Reduce(&e1, nullptr, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

            MPI_Bcast(&end_flag, 1, MPI_INT, 0, MPI_COMM_WORLD);
        }
        delete[] buf_a;
        delete[] buf_b;
    }

    MPI_Finalize();

    return 0;
}