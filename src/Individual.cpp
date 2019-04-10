//
// Created by sliberman on 4/5/19.
//

#include <cmath>
#include <iostream>
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

}


Individual::Individual(int number_of_polygons, int number_of_vertices, int max_x, int max_y) {
    this->max_x = max_x;
    this->max_y = max_y;
    this->number_of_vertices = number_of_vertices;
    this->polygons = new list<Polygon *>();
    for (int i = 0; i < number_of_polygons; ++i) {
        this->polygons->push_back(Polygon::random_polygon(number_of_vertices, max_x, max_y));
    }
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
            colour->set_a(1 - utils::random());
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
    this->polygons->erase(it);
}

int Individual::get_len_dna(){
    return this->polygons->size();
}

void Individual::draw(unsigned char *canvas, int width, int height) {
    for (int i = 0; i < width * height; ++i) {
        canvas[i] = 0;
    }

    Colour *c;
    for (auto & polygon : *this->polygons) {
        c = polygon->colour;
        for (int k = 0; k < polygon->get_points_length(); ++k) {
            Point *p1 = polygon->get_point(k);
            Point *p2 = k + 1 < polygon->get_points_length() ? polygon->get_point(k + 1) : polygon->get_point(0);
            int x_1 = p1->get_x();
            int x_2 = p2->get_x();
            int y_1 = p1->get_y();
            int y_2 = p2->get_y();
            double m = (double)(y_2 - y_1) / (double)(x_2 - x_1);
            double b = y_2 - m * x_2;
            int M_x = max(x_1, x_2);
            int m_x = x_1 + x_2 - M_x;
            for (int i = m_x; i < M_x; ++i) {
                int y = (int)round(m_x * m + b);
                canvas[(i + y * width) * 4] = c->get_r();
                canvas[(i + y * width) * 4 + 1] = c->get_g();
                canvas[(i + y * width) * 4 + 2] = c->get_b();
                canvas[(i + y * width) * 4 + 3] = (int)round(255 * c->get_a());
            }
        }

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
                    canvas[(i + j * width) * 4] = c->get_r();
                    canvas[(i + j * width) * 4 + 1] = c->get_g();
                    canvas[(i + j * width) * 4 + 2] = c->get_b();
                    canvas[(i + j * width) * 4 + 3] = (int)round(255 * c->get_a());
                }
            }
        }
//        cout << "Counts: " << cnt << endl;
    }
}

Individual::Individual(Individual *original) {
    this->max_x = original->max_x;
    this->max_y = original->max_y;
    this->polygons = new list<Polygon *>();
    for (int i = 0; i < original->get_len_dna(); ++i) {
        this->polygons->push_back(new Polygon(original->get_dna(i)));
    }
    this->number_of_vertices = original->number_of_vertices;
    this->fitness = 0;
}

Individual::~Individual() {
    for (auto & polygon : *this->polygons) {
        delete polygon;
    }
}
