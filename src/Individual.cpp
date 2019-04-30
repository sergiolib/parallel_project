//
// Created by sliberman on 4/5/19.
//

#include <opencv2/opencv.hpp>
#include <cmath>
#include <iostream>
#include "mpi.h"
#include "utils.h"
#include "Individual.h"
#include "conf.h"

using namespace std;

Individual::Individual(int number_of_vertices, int max_x, int max_y,
                       list<Polygon *> *cloned_polys) {
    this->max_x = max_x;
    this->max_y = max_y;
    this->polygons = cloned_polys;
    this->number_of_vertices = number_of_vertices;
    this->fitness = 0;
}


Individual::Individual(int number_of_polygons, int number_of_vertices, int max_x, int max_y) {
    this->max_x = max_x;
    this->max_y = max_y;
    this->number_of_vertices = number_of_vertices;
    this->polygons = new list<Polygon *>();
    for (int i = 0; i < number_of_polygons; ++i) {
        this->polygons->push_back(Polygon::random_polygon(number_of_vertices, max_x, max_y));
    }
    this->fitness = 0;
}

void Individual::mutate() {
    int len = this->polygons->size();

    int idx1 = utils::next_int(len - 1);
    Polygon *poly = this->get_dna(idx1);
    double r = utils::random();

    if (r < 0.45) {
        Colour *colour = poly->colour;
        if (r < 0.1125) {
            colour->set_r(Colour::random_colour());
        } else if (r < 0.2250) {
            colour->set_g(Colour::random_colour());
        } else if (r < 0.3375) {
            colour->set_b(Colour::random_colour());
        } else {
            colour->set_a(1.0 - utils::random());
        }
    } else if (r < 0.9) {
        if (r < 0.675) {
            poly->move(this->max_x, this->max_y);
        } else {
            Point *point = poly->get_point(utils::next_int(poly->get_points_length() - 1));
            if (r < 0.7875) {
                point->set_x(utils::next_int(this->max_x));
            } else {
                point->set_y(utils::next_int(this->max_y));
            }
        }
    } else if (r < 0.95) {
        if (r < 0.925) {
            poly->remove_point();
        } else {
            int len2 = poly->get_points_length();
            if (len2 < max_number_of_vertices) {
                poly->insert_point(utils::next_int(len2),
                        new Point(utils::next_int(this->max_x), utils::next_int(this->max_y)));
            }
        }
    } else {
        if (r < 0.975) {
            if (len >= 2) {
                this->remove_dna(idx1);
            }
        } else {
            if (len < max_number_of_polygons) {
                this->insert_dna(utils::next_int(len),
                        Polygon::random_polygon(this->number_of_vertices, max_x, max_y));
            }
        }
    }
}

Polygon *Individual::get_dna(int index) {
    return *next(this->polygons->begin(), index);
}

void Individual::insert_dna(int index, Polygon *p) {
    auto it = this->polygons->begin();
    advance(it, index - 1);
    this->polygons->insert(it, p);
}

void Individual::remove_dna(int index) {
    auto it = this->polygons->begin();
    advance(it, index);
    Polygon *elem = *it;
    this->polygons->erase(it);
    delete elem;
}

int Individual::get_len_dna(){
    return this->polygons->size();
}

void Individual::draw_CPU(unsigned char *canvas, int width, int height) {
    for (int i = 0; i < width * height * 4; ++i) {
        canvas[i] = 0;
    }

    Colour *c;
    for (auto & polygon : *this->polygons) {
        c = polygon->colour;
        //        int cnt = 0;
        int min_x_this_pol = width;
        int max_x_this_pol = 0;
        int min_y_this_pol = height;
        int max_y_this_pol = 0;

        for (int k = 0; k < polygon->get_points_length(); ++k) {
            min_x_this_pol = min(polygon->get_point(k)->get_x(), min_x_this_pol);
            max_x_this_pol = max(polygon->get_point(k)->get_x(), max_x_this_pol);
            min_y_this_pol = min(polygon->get_point(k)->get_y(), min_y_this_pol);
            max_y_this_pol = max(polygon->get_point(k)->get_y(), max_y_this_pol);
        }

        for (int i = min_x_this_pol; i < max_x_this_pol; i++) {
            for (int j = min_y_this_pol; j < max_y_this_pol; j++) {
                if (utils::is_in_polygon(i, j, *polygon)) {
//                    cnt++;
                    canvas[(i + j * width) * 4] = (unsigned char)(canvas[(i + j * width) * 4] * (1 - c->get_a())) + (unsigned char)(c->get_r() * c->get_a());
                    canvas[(i + j * width) * 4 + 1] = (unsigned char)(canvas[(i + j * width) * 4 + 1] * (1 - c->get_a())) + (unsigned char)(c->get_g() * c->get_a());
                    canvas[(i + j * width) * 4 + 2] = (unsigned char)(canvas[(i + j * width) * 4 + 2] * (1 - c->get_a())) + (unsigned char)(c->get_b() * c->get_a());
                }
            }
        }
//        cout << "Counts: " << cnt << endl;
    }
}

void Individual::draw_CV(unsigned char *canvas, int width, int height) {
    for (int j = 0; j < width * height * 4; ++j) {
        canvas[j] = 0;
    }
    cv::Mat final_img = cv::Mat(height, width, CV_8UC4, canvas);
    for (auto & polygon : *this->polygons) {
        cv::Mat partial_img = final_img.clone();
        Colour *color = polygon->colour;
        cv::Scalar color_s = cv::Scalar(color->get_b(), color->get_g(), color->get_r());
        int n_pts[] = {polygon->get_points_length()};
        cv::Point *pts = new cv::Point[*n_pts];
        for (int i = 0; i < *n_pts; ++i) {
            Point *p = polygon->get_point(i);
            pts[i] = cv::Point(p->get_x(), p->get_y());
        }
        const cv::Point* ppt[1] = {pts};
        cv::fillPoly(partial_img, ppt, n_pts, 1, color_s, cv::LINE_AA);

        cv::addWeighted(partial_img, color->get_a(), final_img, 1.0 - color->get_a(), 0.0, final_img);

        delete[] pts;

//        cv::namedWindow("Hola", cv::WINDOW_AUTOSIZE);
//        cv::imshow("Hola", final_img);

//        cv::waitKey(0);
    }
}

void wake_workers() {
  // Wake slaves
  int flag = 0;

  //    cout << "rank " << rank << ": waiting for flag" << endl;
  MPI_Bcast(&flag, 1, MPI_INT, 0, MPI_COMM_WORLD);
  //    cout << "rank: " << rank << ": flag is " << flag << endl;
}

void Individual::draw_CV_parallel(unsigned char *canvas, unsigned char *buf, int *buf_ind, int width, int height) {
    int P;
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    int dims[2];
    dims[0] = width;
    dims[1] = height;
    int whole_len = width * height * 4;
    int len_each = width * (height / P) * 4;

    wake_workers();

    // Send dims
    // cout << "rank " << rank << ": sending dims" << endl;
    MPI_Bcast(&dims, 2, MPI_INT, 0, MPI_COMM_WORLD);
    // cout << "rank: " << rank << ": dims are " << dims[0] << "x" << dims[1] << endl;

    // cout << "rank " << rank << ": sending scattered canvas" << endl;
    //cout << "1:0 sending " << len_each << endl;
    MPI_Scatter(canvas, len_each, MPI_UNSIGNED_CHAR, buf, len_each, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    //cout << "1:0 sent " << len_each << endl;
    int small = len_each * P;
    //cout << "2:0 sending " << whole_len - small << endl;
    MPI_Send(canvas + small, whole_len - small, MPI_UNSIGNED_CHAR, P - 1, 123, MPI_COMM_WORLD);
    //cout << "2:0 sent " << whole_len - small << endl;
    // cout << "rank " << rank << ": canvas sent" << endl;

    int new_height = height / P;

    for (int j = 0; j < len_each; ++j) {
        buf[j] = 0;
    }
    cv::Mat img = cv::Mat(new_height, width, CV_8UC4, buf);

    // Serialize individuals and send them
    // Format: [len_arr [n_pts r g b a [px py]...]...]

    int n_polys = this->get_len_dna();

    int sz_buf_ind = 0;
    for (int k = 0; k < n_polys; ++k) {
        buf_ind[sz_buf_ind++] = this->get_dna(k)->get_points_length();
        buf_ind[sz_buf_ind++] = this->get_dna(k)->colour->get_r();
        buf_ind[sz_buf_ind++] = this->get_dna(k)->colour->get_g();
        buf_ind[sz_buf_ind++] = this->get_dna(k)->colour->get_b();
        buf_ind[sz_buf_ind++] = (int)(255*this->get_dna(k)->colour->get_a());
        for (int i = 0; i < this->get_dna(k)->get_points_length(); ++i) {
            buf_ind[sz_buf_ind++] = this->get_dna(k)->get_point(i)->get_x();
            buf_ind[sz_buf_ind++] = this->get_dna(k)->get_point(i)->get_y();
        }
    }

    // cout << "rank " << rank << ": sending ind" << endl;
    MPI_Bcast(&sz_buf_ind, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(buf_ind, sz_buf_ind, MPI_INT, 0, MPI_COMM_WORLD);
    // cout << "rank " << rank << ": sent ind" << endl;

    int r, g, b, a, x, y;
    int cur_off = 0;
    while (cur_off < sz_buf_ind) {
        cv::Mat partial_img = img.clone();
        int n_pts[] = {buf_ind[cur_off++]};
        r = buf_ind[cur_off++];
        g = buf_ind[cur_off++];
        b = buf_ind[cur_off++];
        a = buf_ind[cur_off++];
        cv::Scalar color_s = cv::Scalar(b, g, r);
        auto pts = new cv::Point[*n_pts];
        for (int i = 0; i < *n_pts; ++i) {
            x = buf_ind[cur_off++];
            y = buf_ind[cur_off++];
            pts[i] = cv::Point(x, y);
        }
        const cv::Point* ppt[1] = {pts};
        cv::fillPoly(partial_img, ppt, n_pts, 1, color_s, cv::LINE_AA);

        cv::addWeighted(partial_img, ((double) a) / 255.0, img, 1.0 - ((double) a) / 255.0, 0.0, img);

        delete[] pts;

    }
    
    //cv::imwrite("part_" + to_string(0) + ".bmp", img);

    // cout << "rank " << rank << ": receiving canvas" << endl;
    //cout << "3:0 receiving " << len_each << endl;  
    MPI_Gather(buf, len_each, MPI_UNSIGNED_CHAR, 
	       canvas, len_each, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    //cout << "3:0 received " << len_each << endl;  
    MPI_Status status;
    //cout << "4:0 receiving " << whole_len - small << endl;  
    MPI_Recv(canvas + small, whole_len - small, MPI_UNSIGNED_CHAR, P - 1, 123, MPI_COMM_WORLD, &status);
    //cout << "4:0 received " << whole_len - small << endl;  
    // cout << "rank " << rank << ": received canvas!" << endl;

    //auto canvas_img = cv::Mat(height, width, CV_8UC4, canvas);
    //cv::imwrite("full_img.bmp", canvas_img);
}

Individual::Individual(Individual *original) {
    this->max_x = original->max_x;
    this->max_y = original->max_y;
    this->polygons = new list<Polygon *>();
    for (int i = 0; i < original->get_len_dna(); ++i) {
        this->polygons->push_back(new Polygon(original->get_dna(i)));
    }
    this->number_of_vertices = original->number_of_vertices;
    this->fitness = original->fitness;
}

Individual::~Individual() {
    for (auto & polygon : *this->polygons) {
        delete polygon;
    }
}
