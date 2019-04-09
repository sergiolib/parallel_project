//
// Created by sliberman on 4/9/19.
//

#include <random>

using namespace std;

#include "utils.h"

int utils::next_int(int n) {
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
    uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen);
}

double utils::diff(const unsigned char *byte_arr_a, const unsigned char *byte_arr_b, int width, int height) {
    int e1 = 0;
    int len = width * height;
    for (int i = 0; i < len; i += 4) {
        int off1 = i + 1, off2 = i + 2;
        int r_a = byte_arr_a[i], g_a = byte_arr_a[off1], b_a = byte_arr_a[off2];
        int r_b = byte_arr_b[i], g_b = byte_arr_b[off1], b_b = byte_arr_b[off2];
        int r_delta = r_a - r_b;
        int g_delta = g_a - g_b;
        int b_delta = b_a - b_b;
        e1 += abs(r_delta) + abs(g_delta) + abs(b_delta);
    }
    return 1 - ((double)e1 / (double)(255*3*width*height));
}

unsigned char *utils::draw_individuals(vector<Individual *> individuals, int width, int height) {
    auto *res = new unsigned char[width * height];
    for (auto it = individuals.begin(); it != individuals.end(); ++it) {
        (*it)->draw(res, width, height);
    }
    return res;
}
