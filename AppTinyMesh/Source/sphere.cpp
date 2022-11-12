#include "sphere.h"
#include <iostream>

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

Sphere::Sphere(const Vector& p_c, double p_r)
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

bool Sphere::Contains(const Vector& p) const
{
    return (pow(p[0] - c[0], 2.0) + pow(p[1] - c[1], 2.0) + pow(p[2] - c[2], 2.0)) < pow(r, 2.0);
}
