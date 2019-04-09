//
// Created by sliberman on 4/5/19.
//

#include <list>

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
    void move(int max_x, int max_y);
    Point *get_point(int index);
    int get_points_length();
    void remove_point();
    void add_point(Point *point);
    void insert_point(int index, Point *p);

private:
    list<Point *> *points;

};


#endif //PARALLELGENETIC_POLYGON_H
