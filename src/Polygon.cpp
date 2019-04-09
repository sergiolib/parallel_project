//
// Created by sliberman on 4/5/19.
//

#include "utils.h"

using namespace std;

#include "Polygon.h"
#include "Colour.h"

Polygon *Polygon::random_polygon(int numberOfVertices, int maxX, int maxY) {
    unsigned char r, g, b;
    double a;

    r = random_colour();
    g = random_colour();
    b = random_colour();
    a = random_alpha();

    Polygon* poly = new Polygon(new Colour(r, g, b, a));

    for (int i = 0; i < numberOfVertices; ++i) {
        int x = utils::nextInt(maxX);
        int y = utils::nextInt(maxY);
        poly->addPoint(Point(x, y));
    }

    return poly;
}

Polygon::Polygon(Colour *colour) {
    this->colour = colour;

}

void Polygon::addPoint(Point point) {
    this->points.push_back(point);
}
