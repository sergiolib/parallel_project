//
// Created by sliberman on 4/5/19.
//

#include "Colour.h"
#include <random>

using namespace std;

Colour::Colour(unsigned char r, unsigned char g, unsigned char i, double a) {

}

unsigned char Colour::random_colour() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 255);
    return dis(gen);
}

double Colour::random_alpha() {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen);
}

void Colour::set_r(int r) {
    this->r = r;
}

void Colour::set_g(int g) {
    this->g = g;
}

void Colour::set_b(int b) {
    this->b = b;
}

void Colour::set_a(double a) {
    this->a = a;
}

int Colour::get_r() {
    return r;
}

int Colour::get_b() {
    return b;
}

int Colour::get_g() {
    return g;
}

double Colour::get_a() {
    return a;
}
