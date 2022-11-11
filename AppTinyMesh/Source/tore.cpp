#include "tore.h"

Tore::Tore()
{
    c = Vector(0.0, 0.0, 0.0);
    r = 1.0;
    R = 0.5;
}

Tore::Tore(double p_r, double p_R)
{
    c = Vector(0.0, 0.0, 0.0);
    r = p_r;
    R = p_R;
}

Tore::Tore(Vector& p_c, double p_r, double p_R)
{
    c = p_c;
    r = p_r;
    R = p_R;
}

Vector Tore::Center() const
{
    return c;
}

double Tore::Radius1() const
{
    return r;
}

double Tore::Radius2() const
{
    return R;
}
