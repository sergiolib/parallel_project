//
// Created by sliberman on 4/5/19.
//

#include "Polygon.h"

using namespace std;

#ifndef PARALLELGENETIC_INDIVIDUAL_H
#define PARALLELGENETIC_INDIVIDUAL_H


class Individual {
public:
    Individual(int number_of_polygons, int number_of_vertices, int max_x, int max_y, list<Polygon *> *cloned_polys);
    Individual(int number_of_polygons, int number_of_vertices, int max_x, int max_y);
    void mutate();

private:
    list<Polygon *> *dna = new list<Polygon *>();
    double fitness;
    int max_x;
    int max_y;
    Polygon *get_dna(int index);
    void insert_dna(int index, Polygon *p);
    void remove_dna(int index);

    int number_of_vertices;
};


#endif //PARALLELGENETIC_INDIVIDUAL_H
