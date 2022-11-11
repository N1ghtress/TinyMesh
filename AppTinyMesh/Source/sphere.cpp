#include "sphere.h"

Sphere::Sphere()
{
    c = Vector(0.0, 0.0, 0.0);
    up = Vector(0.0, 0.0, 1.0);
    r = 1.0;
}

Sphere::Sphere(double p_r)
{
    c = Vector(0.0, 0.0, 0.0);
    up = Vector(0.0, 0.0, 1.0);
    r = p_r;
}

Sphere::Sphere(Vector& p_c, double p_r)
{
    c = p_c;
    up = Vector(0.0, 0.0, 1.0);
    r = p_r;
}

Vector Sphere::Center() const
{
    return c;
}

Vector Sphere::Normal() const
{
    return up;
}

double Sphere::Radius() const
{
    return r;
}
