#include "cylinder.h"

Cylinder::Cylinder()
{
    c = Vector(0.0, 0.0, 0.0);
    up = Vector(0.0, 0.0, 1.0);
    r = 1.0;
    h = 1.0;
}

Cylinder::Cylinder(double p_r, double  p_h)
{
    c = Vector(0.0, 0.0, 0.0);
    up = Vector(0.0, 0.0, 1.0);
    r = p_r;
    h = p_h;
}

Cylinder::Cylinder(Vector& p_c, double p_r, double p_h)
{
    c = p_c;
    up = Vector(0.0, 0.0, 1.0);
    r = p_r;
    h = p_h;
}

Vector Cylinder::Center() const
{
    return c;
}

Vector Cylinder::Normal() const
{
    return up;
}

double Cylinder::Radius() const
{
    return r;
}

double Cylinder::Height() const
{
    return h;
}
