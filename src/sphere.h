#include <iostream>
#include "shape.h"

#ifndef SPHERE_H
#define SPHERE_H

class Sphere : public Shape {

public:

    Sphere(const Color&, float, float, const Vec3&, float);

    bool intersect(const Vec3&, const Vec3&, Vec3&) const;
    Vec3 normal(const Vec3&) const;

private:

    Vec3 center;
    float radius;
};

#endif
