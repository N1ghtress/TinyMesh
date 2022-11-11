#pragma once

#include "mathematics.h"

class Cone
{
protected:
    Vector c, up;
    double r, h;
public:
    Cone();
    explicit Cone(double, double);
    explicit Cone(const Vector&, double, double);

    ~Cone() {}

    Vector Center() const;
    Vector Normal() const;
    double Radius() const;
    double Height() const;
};
