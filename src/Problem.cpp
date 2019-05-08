//
// Created by liberman and ayin on 4/5/19.
//

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include "Problem.h"

using namespace std;

void Problem::run(cv::Mat *img, int max_epochs, bool use_mpi, int channels, string output_filename) {
    cv::Size s = img->size();
    GeneticAlgorithm ga = GeneticAlgorithm(img->data, s.width, s.height, channels);
    Individual *result = ga.evolve(max_epochs, use_mpi);
    cv::Mat result_mat;
    if (channels == 4) {
        result_mat = cv::Mat(s.height, s.width, CV_8UC4);
    } else {
        result_mat = cv::Mat(s.height, s.width, CV_8UC3);
    }
    result->draw_CV(result_mat.data, s.width, s.height, channels);

    if (channels == 4) {
        cvtColor(result_mat, result_mat, cv::COLOR_BGRA2BGR);
    }
    imwrite(output_filename, result_mat);
}
