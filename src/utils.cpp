//
// Created by sliberman on 4/9/19.
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
    return (double)rand() / (double)RAND_MAX;
}

void utils::set_seed(int seed) {
    srand(seed);
}

void wake_workers_tmp() {
    // Wake slaves
    int flag = 0;

    //    cout << "rank " << rank << ": waiting for flag" << endl;
    MPI_Bcast(&flag, 1, MPI_INT, 0, MPI_COMM_WORLD);
    //    cout << "rank: " << rank << ": flag is " << flag << endl;
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
    return 1.0 - e1 / (255.0*channels*width*height);
}

double utils::diff_parallel(unsigned char *byte_arr_a, unsigned char *byte_arr_b, unsigned char *buf_a, int width, int height, int channels) {
    // Function executed by root
    int P, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int e1 = 0;
    int len_each = height / P * width * channels;
    int residual = height * width * channels - len_each * P;

//    int i = 0;
//    while (i==0) {
//        sleep(5);
//    }
//
//    cout << "height" << height << endl;
//    cout << "width" << width << endl;

    wake_workers_tmp();

//    cout << "rank: " << rank << ": pegado en a?" << endl;

//    cv::Mat kenny0(height / 4, width, CV_8UC4, byte_arr_b);
//    cv::Mat kenny1(height / 4, width, CV_8UC4, byte_arr_b + (height / 4 * width) * 4 * 1);
//    cv::Mat kenny2(height / 4, width, CV_8UC4, byte_arr_b + (height / 4 * width) * 4 * 2);
//    cv::Mat kenny3(height / 4, width, CV_8UC4, byte_arr_b + (height / 4 * width) * 4 * 3);
//    cv::namedWindow("Hola", cv::WINDOW_AUTOSIZE);
//    cv::imshow("Hola", kenny0);
//    cv::waitKey(0);
//    cv::namedWindow("Hola", cv::WINDOW_AUTOSIZE);
//    cv::imshow("Hola", kenny1);
//    cv::waitKey(0);
//    cv::namedWindow("Hola", cv::WINDOW_AUTOSIZE);
//    cv::imshow("Hola", kenny2);
//    cv::waitKey(0);
//    cv::namedWindow("Hola", cv::WINDOW_AUTOSIZE);
//    cv::imshow("Hola", kenny3);
//    cv::waitKey(0);

    MPI_Scatter(byte_arr_a, len_each, MPI_UNSIGNED_CHAR, buf_a, len_each, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    MPI_Send(byte_arr_a+len_each*P, residual, MPI_UNSIGNED_CHAR, P - 1, 123, MPI_COMM_WORLD);
//    cout << "rank: " << rank << ": no pegado en a" << endl;

//    cout << "rank: " << rank << ": pegado en b?" << endl;
//    MPI_Scatter(byte_arr_b, len_each, MPI_UNSIGNED_CHAR, buf_b, len_each, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
//    MPI_Send(byte_arr_b+len_each*P, residual, MPI_UNSIGNED_CHAR, P - 1, 123, MPI_COMM_WORLD);
//    cout << "rank: " << rank << ": no pegado en b" << endl;

//    auto img1 = cv::Mat(height / P, width, CV_8UC4, buf_a);
//    cv::imwrite("im_a_0.bmp", img1);
//
//    auto img2 = cv::Mat(height / P, width, CV_8UC4, buf_b);
//    cv::imwrite("im_b_0.bmp", img2);

    int sum = 0, v1, v2;
    for (int i = 0; i < len_each; i++) {
        v1 = buf_a[i];
        v2 = byte_arr_b[i];
        e1 += abs(v1 - v2);
    }
//    cout << "rank " << rank << ": " << e1 << endl;
    MPI_Reduce(&e1, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
//    cout << "Whole sum: " << sum << endl;

    return 1.0 - sum / (255.0*channels*width*height);
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
