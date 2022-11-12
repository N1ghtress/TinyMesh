#pragma once

#include "mathematics.h"

class Capsule
{
protected:
    Vector c, up;
    double r, h;
public:
    Capsule();
    explicit Capsule(double, double);
    explicit Capsule(const Vector&, double, double);

    ~Capsule() {}

    Vector Center() const;
    Vector Normal() const;
    double Radius() const;
    double Height() const;
};
