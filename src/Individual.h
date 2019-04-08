//
// Created by sliberman on 4/5/19.
//

#include <vector>
#include "Polygon.h"

using namespace std;

#ifndef PARALLELGENETIC_INDIVIDUAL_H
#define PARALLELGENETIC_INDIVIDUAL_H


class Individual {
public:
    Individual();

private:
    vector<Polygon> dna;
};


#endif //PARALLELGENETIC_INDIVIDUAL_H
