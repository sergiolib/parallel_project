//
// Created by liberman and ayin on 4/9/19.
//

#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <unistd.h>
#include "mpi.h"

using namespace std;

#include "utils.h"

int utils::next_int(int n) {
    /*
     * Get a random number up until n.
     */

    return rand() % (n + 1);
}

double utils::random() {
    return (double) rand() / (double) RAND_MAX;
}

void utils::set_seed(int seed) {
    srand(seed);
}

void wake_workers_tmp() {
    int flag = 0;
    MPI_Bcast(&flag, 1, MPI_INT, 0, MPI_COMM_WORLD);
}

double utils::diff(unsigned char *byte_arr_a, unsigned char *byte_arr_b, int width, int height, int channels) {
    int e1 = 0;
    int len = width * height * channels;
    int v1, v2;
    for (int i = 0; i < len; i++) {
        v1 = byte_arr_a[i];
        v2 = byte_arr_b[i];
        e1 += abs(v1 - v2);
    }
    return 1.0 - e1 / (255.0 * len);
}

double
utils::diff_parallel(unsigned char *byte_arr_a, unsigned char *byte_arr_b, unsigned char *buf_a, int width, int height,
                     int channels) {
    int P, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int e1 = 0;
    int t = width * channels;
    int len_each = height / P * t;

    MPI_Request r1, r2;
    wake_workers_tmp();
    MPI_Iscatter(byte_arr_a, len_each, MPI_UNSIGNED_CHAR, buf_a, len_each, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD, &r1);

    int residual = height * t - len_each * P;

    MPI_Isend(byte_arr_a + len_each * P, residual, MPI_UNSIGNED_CHAR, P - 1, 123, MPI_COMM_WORLD, &r2);

    int sum = 0, v1, v2;
    for (int i = 0; i < len_each; i++) {
        v1 = byte_arr_a[i];
        v2 = byte_arr_b[i];
        e1 += abs(v1 - v2);
    }

    MPI_Status s;
    MPI_Wait(&r1, &s);
    MPI_Wait(&r2, &s);

    MPI_Reduce(&e1, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return 1.0 - sum / (255.0 * channels * width * height);
}

bool utils::is_in_polygon(int x, int y, Polygon polygon) {
    bool is_in = false;
    int num_points = polygon.get_points_length();
    for (int i = 0, j = num_points - 1; i < num_points; j = i++) {
        Point *p1 = polygon.get_point(i);
        Point *p2 = polygon.get_point(j);
        if (((p1->get_y() >= y) != (p2->get_y() >= y)) &&
            (x <= (p2->get_x() - p1->get_x()) * (y - p1->get_y()) / (p2->get_y() - p1->get_y()) + p1->get_x())
                ) {
            is_in = !is_in;
        }
    }
    return is_in;
}
