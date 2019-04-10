//
// Created by sliberman on 4/5/19.
//

#include <values.h>
#include "utils.h"
#include "Polygon.h"
#include "Colour.h"

using namespace std;

Polygon *Polygon::random_polygon(int number_of_vertices, int max_x, int max_y) {
    unsigned char r, g, b;
    double a;

    r = Colour::random_colour();
    g = Colour::random_colour();
    b = Colour::random_colour();
    a = Colour::random_alpha();

    auto poly = new Polygon(new Colour(r, g, b, a));

    for (int i = 0; i < number_of_vertices; ++i) {
        int x = utils::next_int(max_x);
        int y = utils::next_int(max_y);
        poly->add_point(new Point(x, y));
    }

    return poly;
}

Polygon::Polygon(Colour *colour) {
    this->colour = colour;
    this->points = new list<Point *>();
}

Polygon::Polygon(Polygon *original) {
    this->colour = new Colour(original->colour);
    this->points = new list<Point *>();
    for (auto & point : *this->points) {
        this->points->push_back(new Point(point));
    }
}

void Polygon::add_point(Point *point) {
    this->points->push_back(point);
}

void Polygon::move(int max_x, int max_y) {
    int ne_x = 0;
    int ne_y = MAXINT;
    int sw_x = MAXINT;
    int sw_y = 0;

    int len = this->get_points_length();
    for (int i = 0; i < len; ++i) {
        Point *vertice = this->get_point(i);
        int x = vertice->get_x();
        int y = vertice->get_y();

        if (x > ne_x) {
            ne_x = x;
        }
        if (x < sw_x) {
            sw_x = x;
        }
        if (y < ne_y) {
            ne_y = y;
        }
        if (y > sw_y) {
            sw_y = y;
        }
    }

    int w = ne_x - sw_x;
    int h = sw_y - ne_y;

    int ne_x_new = w + utils::next_int(max_x - w);
    int ne_y_new = utils::next_int(max_y - h);

    int delta_x = ne_x_new - ne_x;
    int delta_y = ne_y_new - ne_y;

    for (int i = 0; i < len; ++i) {
        Point *vertice = this->get_point(i);
        vertice->set_x(vertice->get_x() + delta_x);
        vertice->set_y(vertice->get_y() + delta_y);
    }
}

Point *Polygon::get_point(int index) {
    return *next(points->begin(), index);
}

int Polygon::get_points_length() {
    return this->points->size();
}

void Polygon::remove_point() {
    int len = this->points->size();
    if (len > 3) {
        auto it = next(this->points->begin(), utils::next_int(len - 1));
        this->points->erase(it);
    }
}

void Polygon::insert_point(int index, Point *p) {
    auto it = this->points->begin();
    advance(it, index - 1);
    this->points->insert(it, p);
}
