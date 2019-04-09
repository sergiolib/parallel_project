//
// Created by sliberman on 4/9/19.
//

#ifndef PARALLELGENETIC_UTILS_H
#define PARALLELGENETIC_UTILS_H


#include "Individual.h"

class utils {
public:
    static int next_int(int n);
    static double random();
    static double diff(const unsigned char *byte_arr_a, const unsigned char *byte_arr_b, int width, int height);
    static unsigned char draw_individuals(vector<Individual *> individuals, int width, int height);
};


#endif //PARALLELGENETIC_UTILS_H
