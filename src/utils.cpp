//
// Created by sliberman on 4/9/19.
//

#include <random>
#include "mpi.h"

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

double utils::diff_parallel(const unsigned char *byte_arr_a, const unsigned char *byte_arr_b, int width, int height) {
    // Function executed by root
    int P, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int e1 = 0;
    int len = width * height * 4;
    int len_each = len/P;

    // Wake slaves
    int flag = 0;

//    cout << "rank " << rank << ": waiting for flag" << endl;
    MPI_Bcast(&flag, 1, MPI_INT, 0, MPI_COMM_WORLD);
//    cout << "rank: " << rank << ": flag is " << flag << endl;

    auto buf_a = new unsigned char[len_each];
//    cout << "rank: " << rank << ": pegado en a?" << endl;
    MPI_Scatter(byte_arr_a, len_each, MPI_UNSIGNED_CHAR, buf_a, len_each, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
//    cout << "rank: " << rank << ": no pegado en a" << endl;

    auto buf_b = new unsigned char[len_each];
//    cout << "rank: " << rank << ": pegado en b?" << endl;
    MPI_Scatter(byte_arr_b, len_each, MPI_UNSIGNED_CHAR, buf_b, len_each, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
//    cout << "rank: " << rank << ": no pegado en b" << endl;

    int sum = 0;
    for (int i = 0; i < len_each; i++) {
        e1 += abs(buf_a[i] - buf_b[i]);
    }
//    cout << "rank " << rank << ": " << e1 << endl;
    MPI_Reduce(&e1, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
//    cout << "Whole sum: " << sum << endl;

    delete[] buf_a;
    delete[] buf_b;

    return 1 - ((double)sum / (double)(255*4*width*height));
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
