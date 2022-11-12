#pragma once

#include "mathematics.h"

class Sphere
{
protected:
    Vector c, up;
    double r;
public:
    Sphere();
    explicit Sphere(double);
    explicit Sphere(Vector&, double);

    ~Sphere() {}

    Vector Center() const;
    Vector Normal() const;
    double Radius() const;

    bool Contains(const Vector&) const;
};
