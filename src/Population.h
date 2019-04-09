//
// Created by sliberman on 4/5/19.
//

#include <vector>
#include "Individual.h"

using namespace std;

#ifndef PARALLELGENETIC_POPULATION_H
#define PARALLELGENETIC_POPULATION_H


class Population {
public:
    Population(int numberOfIndividuals, int initialNumberOfPolygons, int initialNumberOfVertics, int maxX,
            int maxY);


private:
    vector<Individual> individuals;
    int max = 0;
    //auto elite=null;
    int s = 0; //fitness sum - sum ranks
};


#endif //PARALLELGENETIC_POPULATION_H
