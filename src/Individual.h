//
// Created by sliberman on 4/5/19.
//

#include "Polygon.h"

using namespace std;

#ifndef PARALLELGENETIC_INDIVIDUAL_H
#define PARALLELGENETIC_INDIVIDUAL_H


class Individual {
public:
    explicit Individual(Individual *original);
    ~Individual();
    Individual(int number_of_vertices, int max_x, int max_y, list<Polygon *> *cloned_polys);
    Individual(int number_of_polygons, int number_of_vertices, int max_x, int max_y);
    void mutate();
    Polygon *get_dna(int index);
    void insert_dna(int index, Polygon *p);
    void remove_dna(int index);
    double fitness;
    int get_len_dna();
    void draw_CPU(unsigned char *canvas, int width, int height, int channels);
    void draw_CV(unsigned char *canvas, int width, int height, int channels);
    void draw_CV_parallel(unsigned char *canvas, unsigned char *buf, int *buf_ind, int width, int height, int channels);
    bool operator < (const Individual& str) const
    {
        return (fitness < str.fitness);
    }
    list<Polygon *> *polygons;

private:

    int max_x;
    int max_y;

    int number_of_vertices;
};


#endif //PARALLELGENETIC_INDIVIDUAL_H
