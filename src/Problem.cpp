//
// Created by sliberman on 4/5/19.
//

#include "Problem.h"

Problem::Problem(Mat *img) {
    this->img = img;
    this->ga = new GeneticAlgorithm();
}