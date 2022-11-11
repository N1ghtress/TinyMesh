#pragma once

#include "mathematics.h"

class Disc
{
protected:
    Vector c, up;
    double r;
public:
    Disc();
    explicit Disc(double);
    explicit Disc(const Vector&, double);

    ~Disc() {}

    Vector Center() const;
    Vector Normal() const;
    double Radius() const;
};
