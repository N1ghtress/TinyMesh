#include "disc.h"

Disc::Disc()
{
    c = Vector(0.0, 0.0, 0.0);
    up = Vector(0.0, 1.0, 0.0);
    r = 1.0;
}

Disc::Disc(double p_r)
{
    c = Vector(0.0, 0.0, 0.0);
    up = Vector(0.0, 0.0, 1.0);
    r = p_r;
}

Disc::Disc(const Vector& p_c, double p_r)
{
    c = p_c;
    up = Vector(0.0, 0.0, 1.0);
    r = p_r;
}

Vector Disc::Center() const
{
    return c;
}

Vector Disc::Normal() const
{
    return up;
}

double Disc::Radius() const
{
    return r;
}
