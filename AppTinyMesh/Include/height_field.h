#pragma once

#include "mesh.h"

#include<string>
#include<vector>

class HeightField
{
protected:
    std::vector<int> points;
    double scale;
public:
    HeightField() {};
    explicit HeightField(const char*);
    explicit HeightField(const char*, double);

    ~HeightField() {};

    std::vector<int>& get_points();
    void load(const char*);

    double get_scale();
    void set_scale(double);

    Mesh get_mesh();
};
