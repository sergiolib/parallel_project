//
// Created by liberman and ayin on 4/5/19.
//

#ifndef PARALLELGENETIC_COLOUR_H
#define PARALLELGENETIC_COLOUR_H


class Colour {

public:
    Colour(unsigned char r, unsigned char g, unsigned char i, double a);

    Colour(Colour *original);

    static unsigned char random_colour();

    static double random_alpha();

    void set_r(int r);

    void set_g(int g);

    void set_b(int b);

    void set_a(double a);

    int get_r();

    int get_g();

    int get_b();

    double get_a();

private:
    unsigned char r, g, b;
    double a;
};


#endif //PARALLELGENETIC_COLOUR_H
