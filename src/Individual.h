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
    Polygon *get_dna(int index);
    void insert_dna(int index, Polygon *p);
    void remove_dna(int index);
    double fitness;
    int get_len_dna();
    void draw(unsigned char *canvas, int width, int height);

private:
    list<Polygon *> *dna = new list<Polygon *>();

    int max_x;
    int max_y;

    int number_of_vertices;
};


#endif //PARALLELGENETIC_INDIVIDUAL_H
