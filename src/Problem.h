//
// Created by sliberman on 4/5/19.
//

#include <string>
#include "GeneticAlgorithm.h"
#include <opencv2/opencv.hpp>

using namespace std;

#ifndef PARALLELGENETIC_PROBLEM_H
#define PARALLELGENETIC_PROBLEM_H


class Problem {
public:
    static void run(cv::Mat *img, int max_epochs, bool use_mpi, int channels, string output_filename);
private:
    GeneticAlgorithm *ga;
};


#endif //PARALLELGENETIC_PROBLEM_H
