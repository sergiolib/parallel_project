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
    Population(int number_of_individuals, int initial_number_of_polygons,
            int initial_number_of_vertices, int max_x, int max_y);
    double max;
    vector<Individual *> get_individuals();
    Individual *elite = nullptr;
    int s = 0; //fitness sum - sum ranks

private:
    vector<Individual *> individuals;

};


#endif //PARALLELGENETIC_POPULATION_H
