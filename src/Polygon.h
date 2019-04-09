//
// Created by sliberman on 4/5/19.
//

#include <vector>

using namespace std;

#ifndef PARALLELGENETIC_POLYGON_H
#define PARALLELGENETIC_POLYGON_H


#include "Colour.h"
#include "Point.h"

class Polygon {
public:
    Polygon(Colour *colour);
    static Polygon* random_polygon(int numberOfVertices, int maxX, int maxY);
    Colour *colour;

private:
    vector<Point> points;

    void addPoint(Point point);
};


#endif //PARALLELGENETIC_POLYGON_H
