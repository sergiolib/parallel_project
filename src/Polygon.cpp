//
// Created by sliberman on 4/5/19.
//

#include "utils.h"
#include "Polygon.h"
#include "Colour.h"

using namespace std;

Polygon *Polygon::random_polygon(int numberOfVertices, int maxX, int maxY) {
    unsigned char r, g, b;
    double a;

    r = Colour::random_colour();
    g = Colour::random_colour();
    b = Colour::random_colour();
    a = Colour::random_alpha();

    Polygon *poly = new Polygon(new Colour(r, g, b, a));

    for (int i = 0; i < numberOfVertices; ++i) {
        int x = utils::nextInt(maxX);
        int y = utils::nextInt(maxY);
        poly->add_point(new Point(x, y));
    }

    return poly;
}

Polygon::Polygon(Colour *colour) {
    this->colour = colour;

}

void Polygon::add_point(Point *point) {
    this->points->push_back(point);
}

void Polygon::move(int max_x, int max_y) {

}

Point *Polygon::get_point(int index) {
    return *next(points->begin(), index);
}

int Polygon::get_points_length() {
    return this->points->size()
}

void Polygon::remove_point() {
    int len = this->points->size();
    if (len > 3) {
        auto it = next(this->points->begin(), utils::nextInt(len - 1));
        this->points->erase(it);
    }
}

void Polygon::insert_point(int index, Point *p) {
    auto it = this->points->begin();
    advance(it, index - 1);
    this->points->insert(it, p);
}
