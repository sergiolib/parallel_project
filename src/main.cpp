//
// Created by sliberman on 3/26/19.
//

#include <iostream>
//#include "mpi.h"
#include "GeneticAlgorithm.h"
#include "Problem.h"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

//const int TAG = 123;


int main(int argc, char **argv) {
//    int rank, P, rc;
//    rc = MPI_Init(&argc, &argv);
//    rc = MPI_Comm_size(MPI_COMM_WORLD, &P);
//    rc = MPI_Comm_rank(MPI_COMM_WORLD, &rank);

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
                file = argv[i + 1];
            }
            if (strcmp(argv[i], "-e") == 0) {
                max_epochs = stoi(argv[i + 1]);
            }
            i++;
        }

        Mat image = imread(file, IMREAD_UNCHANGED);

        /* Problem definition */
        /* 1) */
        Problem::run(&image, max_epochs);
//    }

//    MPI_Finalize();

    return 0;
}