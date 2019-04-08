//
// Created by sliberman on 4/5/19.
//

#include <string>
#include "GeneticAlgorithm.h"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#ifndef PARALLELGENETIC_PROBLEM_H
#define PARALLELGENETIC_PROBLEM_H


class Problem {
public:
    Problem(Mat *img);
private:
    string output_directory;
    GeneticAlgorithm *ga;
    Mat *img;
};


#endif //PARALLELGENETIC_PROBLEM_H
