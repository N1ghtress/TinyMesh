#include "tore.h"

Tore::Tore()
{
    c = Vector(0.0, 0.0, 0.0);
    up = Vector(0.0, 0.0, 1.0);
    r = 0.5;
    R = 1.0;
}

Tore::Tore(double p_r, double p_R)
{
    c = Vector(0.0, 0.0, 0.0);
    up = Vector(0.0, 0.0, 1.0);
    r = p_r;
    R = p_R;
}

Tore::Tore(const Vector& p_c, double p_r, double p_R)
{
    c = p_c;
    up = Vector(0.0, 0.0, 1.0);
    r = p_r;
    R = p_R;
}

Vector Tore::Center() const
{
    return c;
}

Vector Tore::Normal() const
{
    return up;
}

double Tore::radius() const
{
    return r;
}

double Tore::Radius() const
{
    return R;
}
