#pragma once

#include "mathematics.h"

class Tore
{
protected:
    Vector c, up;
    double r, R;
public:
    Tore();
    explicit Tore(double, double);
    explicit Tore(const Vector&, double, double);

    ~Tore() {}

    Vector Center() const;
    Vector Normal() const;
    double radius() const; // "Width" radius
    double Radius() const; // "Gap" radius
};
