#pragma once

#include "mathematics.h"

class Tore
{
protected:
    Vector c;
    double r, R;
public:
    Tore();
    explicit Tore(double, double);
    explicit Tore(Vector&, double, double);

    ~Tore() {};

    Vector Center() const;
    double Radius1() const; // r
    double Radius2() const; // R
};
