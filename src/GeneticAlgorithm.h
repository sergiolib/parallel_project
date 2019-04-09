//
// Created by sliberman on 4/5/19.
//

#ifndef PARALLELGENETIC_GENETICALGORITHM_H
#define PARALLELGENETIC_GENETICALGORITHM_H


#include "Population.h"
#include "Individual.h"
#include "utils.h"
#include "conf.h"

class GeneticAlgorithm {
public:
    GeneticAlgorithm();
    vector<Individual> mate(Individual *ind1, Individual *ind2, int numberOfPolygons, int numberOfVertices, int maxX, int maxY);
    void twoPointCrossover(Individual *ind1,Individual *ind2, list<Polygon*> off1, list<Polygon*>off2);
private:
    int indivs;
    int polys ;
    int vertices;
    Population *pop;
};


#endif //PARALLELGENETIC_GENETICALGORITHM_H
