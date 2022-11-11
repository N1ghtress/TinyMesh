#pragma once

#include "mathematics.h"

class Sphere
{
protected:
    Vector c;
    double r;
public:
    Sphere();
    explicit Sphere(double);
    explicit Sphere(Vector&, double);

    ~Sphere() {};

    Vector Center() const;
    double Radius() const;
};
