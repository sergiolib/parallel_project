//
// Created by sliberman on 4/5/19.
//

#include "Population.h"
#include "Individual.h"


Population::Population( int numberOfIndividuals, int initialNumberOfPolygons, int initialNumberOfVertics, int maxX, int maxY) {
    int i;
    for( i = 1; i <= numberOfIndividuals; i++) {
        Individual *individual;
        individual = new Individual(initialNumberOfPolygons, initialNumberOfVertics, maxX, maxY);
        this->individuals.push_back(individual);
        this->s += i;
    }
}
