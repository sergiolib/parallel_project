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
    int len = width * height * 4;
    for (int i = 0; i < len; i += 4) {
        int off1 = i + 1, off2 = i + 2, off3 = i + 3;
        int r_a = byte_arr_a[i], g_a = byte_arr_a[off1], b_a = byte_arr_a[off2], a_a = byte_arr_a[off3];
//        int r_a = byte_arr_a[i], g_a = byte_arr_a[off1], b_a = byte_arr_a[off2];
        int r_b = byte_arr_b[i], g_b = byte_arr_b[off1], b_b = byte_arr_b[off2], a_b = byte_arr_a[off3];
//        int r_b = byte_arr_b[i], g_b = byte_arr_b[off1], b_b = byte_arr_b[off2];
        int r_delta = r_a - r_b;
        int g_delta = g_a - g_b;
        int b_delta = b_a - b_b;
        int a_delta = a_a - a_b;
        e1 += abs(r_delta) + abs(g_delta) + abs(b_delta) + abs(a_delta);
//        e1 += abs(r_delta) + abs(g_delta) + abs(b_delta);
    }
    return 1 - ((double)e1 / (double)(255*4*width*height));
}

bool utils::is_in_polygon(int x, int y, Polygon polygon) {
    bool is_in = false;
    int num_points = polygon.get_points_length();
    for (int i = 0, j = num_points - 1; i < num_points; j = i++) {
        Point *p1 = polygon.get_point(i);
        Point *p2 = polygon.get_point(j);
        if( ( (p1->get_y() >= y ) != (p2->get_y() >= y) ) &&
            (x <= (p2->get_x() - p1->get_x()) * (y - p1->get_y()) / (p2->get_y() - p1->get_y()) + p1->get_x())
                ) {
            is_in = !is_in;
        }
    }
    return is_in;
}
