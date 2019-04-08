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
    Population();

private:
    vector<Individual> individuals;
};


#endif //PARALLELGENETIC_POPULATION_H
