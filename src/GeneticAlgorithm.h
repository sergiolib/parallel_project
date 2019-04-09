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
    mate(Individual ind1, Individual ind2, int numberOfPolygons, int numberOfVertices, int maxX, int maxY);
    twoPointCrossover(Individual ind1,Individual ind2, vector<Polygon>off1, vector<Polygon>off2);
    void evolve(int max_epochs);
private:
    Population *pop;
    unsigned char *data;
};


#endif //PARALLELGENETIC_GENETICALGORITHM_H
