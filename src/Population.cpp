#include <utility>

#include <utility>

#include <utility>

//
// Created by sliberman on 4/5/19.
//

#include "Population.h"
#include "Individual.h"


Population::Population( int number_of_individuals, int initial_number_of_polygons, int initial_number_of_vertices,
        int max_x, int max_y) {
    int i;
    this->max = 0.0;
    this->s = 0;
    for( i = 1; i <= number_of_individuals; i++) {
        auto individual = new Individual(initial_number_of_polygons, initial_number_of_vertices, max_x, max_y);
        this->individuals.push_back(individual);
        this->s += i;
    }
}

void Population::set_individuals(vector<Individual *> ind_vector){
    this->individuals = std::move(ind_vector);
}

vector<Individual *> Population::get_individuals() {
    return this->individuals;
}
