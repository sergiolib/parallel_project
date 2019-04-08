//
// Created by sliberman on 4/5/19.
//

#include <random>

using namespace std;

#include "Polygon.h"
#include "Colour.h"

unsigned char random_colour() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 255);
    return dis(gen);
}

double random_alpha() {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen);
}

int nextInt(int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, max);
    return dis(gen);
}

Polygon *Polygon::random_polygon(int numberOfVertices, int maxX, int maxY) {
    unsigned char r, g, b;
    double a;

    r = random_colour();
    g = random_colour();
    b = random_colour();
    a = random_alpha();

    auto poly = new Polygon(new Colour(r, g, b, a));

    for (int i = 0; i < numberOfVertices; ++i) {
        int x = nextInt(maxX);
        int y = nextInt(maxY);
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
