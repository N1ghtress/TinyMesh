#pragma once

#include "mathematics.h"

class Cylinder
{
protected:
    Vector c, up;
    double r, h;
public:
    Cylinder();
    explicit Cylinder(double, double);
    explicit Cylinder(Vector&, double, double);

    ~Cylinder() {}

    Vector Center() const;
    Vector Normal() const;
    double Radius() const;
    double Height() const;
};
