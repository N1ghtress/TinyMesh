#include "cone.h"

Cone::Cone()
{
    c = Vector(0.0, 0.0, 0.0);
    up = Vector(0.0, 0.0, 1.0);
    r = 1.0;
    h = 1.0;
}

Cone::Cone(double p_r, double p_h)
{
    c = Vector(0.0, 0.0, 0.0);
    up = Vector(0.0, 0.0, 1.0);
    r = p_r;
    h = p_h;
}

Cone::Cone(const Vector& p_c, double p_r, double p_h)
{
    c = p_c;
    up = Vector(0.0, 0.0, 1.0);
    r = p_r;
    h = p_h;
}

Vector Cone::Center() const
{
    return c;
}

Vector Cone::Normal() const
{
    return up;
}

double Cone::Radius() const
{
    return r;
}

double Cone::Height() const
{
    return h;
}
