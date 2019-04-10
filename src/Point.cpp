//
// Created by sliberman on 4/5/19.
//

#include "Point.h"

Point::Point(int x, int y) {
    this->x = x;
    this->y = y;
}

void Point::set_x(int x) {
    this->x = x;
}

void Point::set_y(int y) {
    this->y = y;
}

int Point::get_x() {
    return this->x;
}

int Point::get_y() {
    return this->y;
}

Point::Point(Point *original) {
    this->x = original->x;
    this->y = original->y;
}
