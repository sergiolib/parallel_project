//
// Created by sliberman on 4/5/19.
//

#include "Colour.h"
#include <random>

using namespace std;

Colour::Colour(unsigned char r, unsigned char g, unsigned char b, double a) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
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

void Colour::set_r(int new_r) {
    this->r = new_r;
}

void Colour::set_g(int new_g) {
    this->g = new_g;
}

void Colour::set_b(int new_b) {
    this->b = new_b;
}

void Colour::set_a(double new_a) {
    this->a = new_a;
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

Colour::Colour(Colour *original) {
    this->r = original->r;
    this->g = original->g;
    this->b = original->b;
    this->a = original->a;
}
