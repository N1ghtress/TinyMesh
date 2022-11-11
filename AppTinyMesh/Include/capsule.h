#pragma once

#include "mathematics.h"

class Capsule
{
protected:
    Vector c;
    double r, h;
public:
    Capsule();
    explicit Capsule(double, double);
    explicit Capsule(Vector&, double, double);

    ~Capsule() {}

    Vector Center() const;
    double Radius() const;
    double Height() const;
};
