//
// Created by sliberman on 4/5/19.
//

#ifndef PARALLELGENETIC_POINT_H
#define PARALLELGENETIC_POINT_H


class Point {
public:
    Point(int x, int y);
    void set_x(int x);
    int get_x();
    void set_y(int y);
    int get_y();
private:
    int x;
    int y;
};


#endif //PARALLELGENETIC_POINT_H
