#include <iostream>
#include "vec3.h"
#include "color.h"

#ifndef SHAPE_H
#define SHAPE_H

class Shape {

public:

    virtual bool intersect(const Vec3& p, const Vec3& dir, Vec3& intersection) const = 0;

    virtual Vec3 normal(const Vec3& p) const = 0;

    Shape(const Color& col, float reflCoeff) : color(col), reflCoeff(reflCoeff) {};

    virtual Color getColor(const Vec3&) const {return color;}

    float getReflCoeff() const {return reflCoeff;}

private:

    Color color;
    float reflCoeff;
};

#endif
