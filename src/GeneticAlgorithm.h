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
    GeneticAlgorithm(unsigned char *pixels, int width, int height);
    static vector<Individual *> mate(Individual *ind1, Individual *ind2, int numberOfPolygons, int numberOfVertices, int maxX, int maxY);
    static void twoPointCrossover(Individual *ind1,Individual *ind2, list<Polygon *> off1, list<Polygon *> off2);
    void evolve(int max_epochs);
    Individual* fps(vector<Individual *> indArr, double fitnessSum);
    static void cloneParents(Individual *par1, Individual *par2, list<Polygon *> polys1, list<Polygon *> polys2);
private:
    int indivs;
    int polys ;
    int vertices;
    Population *pop;
    unsigned char *data;
    int width;
    int height;
    int num_polygons;
};


#endif //PARALLELGENETIC_GENETICALGORITHM_H
