//
// Created by sliberman on 4/5/19.
//

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include "Problem.h"

using namespace cv;
using namespace std;

void Problem::run(Mat *img, int max_epochs, bool use_mpi) {
    Size s = img->size();
    GeneticAlgorithm ga = GeneticAlgorithm(img->data, s.width, s.height);
    Individual *result = ga.evolve(max_epochs, use_mpi);
//    result->draw_CPU(canvas, s.width, s.height);
    Mat result_mat = Mat(s.height, s.width, CV_8UC4);
    if (use_mpi) {
        int len_each = s.height / 4 * s.width * 4;
        int residual = s.height * s.width * 4 - len_each * 4;
        auto buf = new unsigned char[len_each + residual];
        int buf_ind[10000];
        result->draw_CV_parallel(result_mat.data, buf, buf_ind, s.width, s.height);
        delete[] buf;
    } else {
        result->draw_CV(result_mat.data, s.width, s.height);
    }


    cvtColor(result_mat, result_mat, COLOR_BGRA2BGR);
    imwrite("output/salida.png", result_mat);

//    namedWindow("Display window");
//    imshow("Display window", result_mat);
//
//    waitKey(0);
//    destroyAllWindows();
}
