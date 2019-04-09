//
// Created by sliberman on 4/5/19.
//

#include <cmath>
#include "utils.h"
#include "Individual.h"
#include "conf.h"

Individual::Individual(int number_of_polygons, int number_of_vertices, int max_x, int max_y,
                       list<Polygon *> *cloned_polys) {
    this->max_x = max_x;
    this->max_y = max_y;
    this->dna = cloned_polys;
}


Individual::Individual(int number_of_polygons, int number_of_vertices, int max_x, int max_y) {
    this->max_x = max_x;
    this->max_y = max_y;
    this->number_of_vertices = number_of_vertices;
    for (int i = 0; i < number_of_polygons; ++i) {
        this->dna->push_back(Polygon::random_polygon(number_of_polygons, max_x, max_y));
    }
}

void Individual::mutate() {
    list<Polygon *> *dna = this->dna;
    int len = dna->size();

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
            Point *point = poly->get_point(utils::next_int(poly->get_points_length()));
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
            if (len2 < conf::max_number_of_vertices) {
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
            if (len < conf::max_number_of_polygons) {
                this->insert_dna(utils::next_int(len),
                        Polygon::random_polygon(this->number_of_vertices, max_x, max_y));
            }
        }
    }
}

Polygon *Individual::get_dna(int index) {
    return *next(this->dna->begin(), index);
}

void Individual::insert_dna(int index, Polygon *p) {
    auto it = this->dna->begin();
    advance(it, index - 1);
    this->dna->insert(it, p);
}

void Individual::remove_dna(int index) {
    auto it = this->dna->begin();
    advance(it, index);
    this->dna->erase(it);
}

int Individual::get_len_dna(){
    return this->dna->size();
}

void Individual::draw(unsigned char *canvas, int width, int height) {
    for (int i = 0; i < width * height; ++i) {
        canvas[i] = 0;
    }

    Colour *c;
    for (auto polygon = this->dna->begin(); polygon != this->dna->end(); ++polygon) {
        c = (*polygon)->colour;
        for (int k = 0; k < (*polygon)->get_points_length(); ++k) {
            Point *p1 = (*polygon)->get_point(k);
            Point *p2 = k + 1 < (*polygon)->get_points_length() ? (*polygon)->get_point(k + 1) : (*polygon)->get_point(0);
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
                canvas[(i + y * width) * 3] = c->get_r();
                canvas[(i + y * width) * 3 + 1] = c->get_b();
                canvas[(i + y * width) * 3 + 2] = c->get_g();
                canvas[(i + y * width) * 3 + 3] = (int)round(255 * c->get_a());
            }
        }

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                if (utils::is_in_polygon(i, j, **polygon)) {
                    canvas[(i + j * width) * 3] = c->get_r();
                    canvas[(i + j * width) * 3 + 1] = c->get_b();
                    canvas[(i + j * width) * 3 + 2] = c->get_g();
                    canvas[(i + j * width) * 3 + 3] = (int)round(255 * c->get_a());
                }
            }
        }
    }
}
