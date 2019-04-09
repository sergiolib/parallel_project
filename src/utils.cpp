//
// Created by sliberman on 4/9/19.
//

#include <random>

using namespace std;

#include "utils.h"

int utils::nextInt(int n) {
    /*
     * Get a random number up until n.
     */

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, n);
    return dis(gen);
}

double utils::random() {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1.0);
    return dis(gen);
}
