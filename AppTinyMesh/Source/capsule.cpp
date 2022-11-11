#include "capsule.h"

Capsule::Capsule()
{
    c = Vector(0.0, 0.0, 0.0);
    up = Vector(0.0, 0.0, 1.0);
    r = 1.0;
    h = 1.0;
}

Capsule::Capsule(double p_r, double p_h)
{
    c = Vector(0.0, 0.0, 0.0);
    up = Vector(0.0, 0.0, 1.0);
    r = p_r;
    h = p_h;
}

Capsule::Capsule(Vector& p_c, double p_r, double p_h)
{
    c = p_c;
    up = Vector(0.0, 0.0, 1.0);
    r = p_r;
    h = p_h;
}

Vector Capsule::Center() const
{
    return c;
}

Vector Capsule::Normal() const
{
    return up;
}

double Capsule::Radius() const
{
    return r;
}

double Capsule::Height() const
{
    return h;
}
