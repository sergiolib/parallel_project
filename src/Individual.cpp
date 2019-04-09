//
// Created by sliberman on 4/5/19.
//

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

    int idx1 = utils::nextInt(len - 1);
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
            Point *point = poly->get_point(utils::nextInt(poly->get_points_length()));
            if (r < 0.7875) {
                point->set_x(utils::nextInt(this->max_x));
            } else {
                point->set_y(utils::nextInt(this->max_y));
            }
        }
    } else if (r < 0.95) {
        if (r < 0.925) {
            poly->remove_point();
        } else {
            int len2 = poly->get_points_length();
            if (len2 < conf::max_number_of_vertices) {
                poly->insert_point(utils::nextInt(len2),
                        new Point(utils::nextInt(this->max_x), utils::nextInt(this->max_y)));
            }
        }
    } else {
        if (r < 0.975) {
            if (len >= 2) {
                this->remove_dna(idx1);
            }
        } else {
            if (len < conf::max_number_of_polygons) {
                this->insert_dna(utils::nextInt(len),
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
