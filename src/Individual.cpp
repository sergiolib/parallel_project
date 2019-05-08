//
// Created by liberman and ayin on 4/5/19.
//

#include <opencv2/opencv.hpp>
#include <cmath>
#include <iostream>
#include <unistd.h>
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

int Individual::get_len_dna() {
    return this->polygons->size();
}

void Individual::draw_CPU(unsigned char *canvas, int width, int height, int channels) {
    for (int i = 0; i < width * height * channels; ++i) {
        canvas[i] = 0;
    }

    Colour *c;
    for (auto &polygon : *this->polygons) {
        c = polygon->colour;
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
                    canvas[(i + j * width) * channels] =
                            (unsigned char) (canvas[(i + j * width) * channels] * (1 - c->get_a())) +
                            (unsigned char) (c->get_r() * c->get_a());
                    canvas[(i + j * width) * channels + 1] =
                            (unsigned char) (canvas[(i + j * width) * channels + 1] * (1 - c->get_a())) +
                            (unsigned char) (c->get_g() * c->get_a());
                    canvas[(i + j * width) * channels + 2] =
                            (unsigned char) (canvas[(i + j * width) * channels + 2] * (1 - c->get_a())) +
                            (unsigned char) (c->get_b() * c->get_a());
                }
            }
        }
    }
}

void Individual::draw_CV(unsigned char *canvas, int width, int height, int channels) {
    for (int j = 0; j < width * height * channels; ++j) {
        canvas[j] = 0;
    }
    cv::Mat final_img;
    if (channels == 4) {
        final_img = cv::Mat(height, width, CV_8UC4, canvas);
    } else {
        final_img = cv::Mat(height, width, CV_8UC3, canvas);
    }
    for (auto &polygon : *this->polygons) {
        cv::Mat partial_img = final_img.clone();
        Colour *color = polygon->colour;
        cv::Scalar color_s = cv::Scalar(color->get_b(), color->get_g(), color->get_r());
        int n_pts[] = {polygon->get_points_length()};
        cv::Point *pts = new cv::Point[*n_pts];
        for (int i = 0; i < *n_pts; ++i) {
            Point *p = polygon->get_point(i);
            pts[i] = cv::Point(p->get_x(), p->get_y());
        }
        const cv::Point *ppt[1] = {pts};
        cv::fillPoly(partial_img, ppt, n_pts, 1, color_s, cv::LINE_4);

        cv::addWeighted(partial_img, color->get_a(), final_img, 1.0 - color->get_a(), 0.0, final_img);

        delete[] pts;
    }
}

void wake_workers() {
    int flag = 0;
    MPI_Bcast(&flag, 1, MPI_INT, 0, MPI_COMM_WORLD);
}

void Individual::draw_CV_parallel(unsigned char *canvas, unsigned char *buf, int *buf_ind, int width, int height,
                                  int channels) {
    int P;
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    int whole_len = width * height * channels;
    int new_height = height / P;
    int len_each = new_height * width * channels;
    int residual = whole_len - len_each * P;

    wake_workers();

    for (int j = 0; j < len_each; ++j) {
        buf[j] = 0;
    }

    cv::Mat img;
    if (channels == 4) {
        img = cv::Mat(new_height, width, CV_8UC4, buf);
    } else {
        img = cv::Mat(new_height, width, CV_8UC3, buf);
    }

    int n_polys = this->get_len_dna();

    int sz_buf_ind = 0;
    for (int k = 0; k < n_polys; ++k) {
        buf_ind[sz_buf_ind++] = this->get_dna(k)->get_points_length();
        buf_ind[sz_buf_ind++] = this->get_dna(k)->colour->get_r();
        buf_ind[sz_buf_ind++] = this->get_dna(k)->colour->get_g();
        buf_ind[sz_buf_ind++] = this->get_dna(k)->colour->get_b();
        buf_ind[sz_buf_ind++] = (int) (255 * this->get_dna(k)->colour->get_a());
        for (int i = 0; i < this->get_dna(k)->get_points_length(); ++i) {
            buf_ind[sz_buf_ind++] = this->get_dna(k)->get_point(i)->get_x();
            buf_ind[sz_buf_ind++] = this->get_dna(k)->get_point(i)->get_y();
        }
    }

    MPI_Request r1, r2;
    MPI_Status s;
    MPI_Ibcast(&sz_buf_ind, 1, MPI_INT, 0, MPI_COMM_WORLD, &r1);
    MPI_Ibcast(buf_ind, sz_buf_ind, MPI_INT, 0, MPI_COMM_WORLD, &r2);

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
        const cv::Point *ppt[1] = {pts};
        cv::fillPoly(partial_img, ppt, n_pts, 1, color_s, cv::LINE_4);

        cv::addWeighted(partial_img, ((double) a) / 255.0, img, 1.0 - ((double) a) / 255.0, 0.0, img);

        delete[] pts;

    }

    MPI_Wait(&r1, &s);
    MPI_Wait(&r2, &s);
    MPI_Igather(buf, len_each, MPI_UNSIGNED_CHAR,
                canvas, len_each, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD, &r1);
    MPI_Irecv(canvas + len_each * P, residual, MPI_UNSIGNED_CHAR, P - 1, 123, MPI_COMM_WORLD, &r2);
    MPI_Wait(&r1, &s);
    MPI_Wait(&r2, &s);
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
    for (auto &polygon : *this->polygons) {
        delete polygon;
    }
}
