//
// Created by sliberman on 4/5/19.
//

#ifndef PARALLELGENETIC_GENETICALGORITHM_H
#define PARALLELGENETIC_GENETICALGORITHM_H


#include "Population.h"
#include "Individual.h"
#include "utils.h"

class GeneticAlgorithm {
public:
    GeneticAlgorithm(unsigned char *pixels, int width, int height, int channels);
    static vector<Individual *> *mate(Individual *ind1, Individual *ind2, int numberOfVertices, int maxX, int maxY);
    static void twoPointCrossover(Individual *ind1,Individual *ind2, list<Polygon *> *off1, list<Polygon *> *off2);
    Individual *evolve(int max_epochs, bool use_mpi);
    static Individual* fps(vector<Individual *> indArr, double fitnessSum);
    static void cloneParents(Individual *par1, Individual *par2, list<Polygon *> *polys1, list<Polygon *> *polys2);
    void clean_population();
private:
    int indivs;
    int polys ;
    int vertices;
    Population *pop;
    unsigned char *data;
    int width;
    int height;
    int channels;
    int num_polygons;

    void check_equal(unsigned char *bytes1, unsigned char *bytes2, int i);
};


#endif //PARALLELGENETIC_GENETICALGORITHM_H
