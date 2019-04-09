//
// Created by sliberman on 4/5/19.
//

#include <vector>
#include "Polygon.h"

using namespace std;

#ifndef PARALLELGENETIC_INDIVIDUAL_H
#define PARALLELGENETIC_INDIVIDUAL_H


class Individual {
public:
    Individual(int number_of_polygons, int number_of_vertices, int max_x, int max_y, vector<Polygon *> *cloned_polys);
    void mutate();

private:
    vector<Polygon *> *dna = new vector<Polygon *>();
    double fitness;
};


#endif //PARALLELGENETIC_INDIVIDUAL_H
