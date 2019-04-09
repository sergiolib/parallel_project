//
// Created by sliberman on 4/5/19.
//

#include "Problem.h"

Problem::Problem(Mat *img, int max_epochs) {
    this->img = img;
    Size s = img->size();
    this->ga = new GeneticAlgorithm(img->data, s.width, s.height);
    this->ga->evolve(max_epochs);
}
