//
// Created by liberman and ayin on 4/9/19.
//

#ifndef PARALLELGENETIC_UTILS_H
#define PARALLELGENETIC_UTILS_H


#include "Individual.h"

class utils {
public:
    static int next_int(int n);

    static double random();

    static void set_seed(int seed);

    static double diff(unsigned char *byte_arr_a, unsigned char *byte_arr_b, int width, int height, int channels);

    static double
    diff_parallel(unsigned char *byte_arr_a, unsigned char *byte_arr_b, unsigned char *buf_a, int width, int height,
                  int channels);

    static bool is_in_polygon(int i, int j, Polygon polygon);
};


#endif //PARALLELGENETIC_UTILS_H
