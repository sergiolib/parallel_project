//
// Created by sliberman on 4/5/19.
//

#include "Point.h"

Point::Point(int x, int y) {
    this->x = x;
    this->y = y;
}

void Point::set_x(int new_x) {
    this->x = new_x;
}

void Point::set_y(int new_y) {
    this->y = new_y;
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
