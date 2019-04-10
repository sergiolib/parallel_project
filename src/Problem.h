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
    static void run(Mat *img, int max_epochs);
private:
    GeneticAlgorithm *ga;
};


#endif //PARALLELGENETIC_PROBLEM_H
