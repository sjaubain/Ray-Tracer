#include <iostream>
#include "shape.h"

#ifndef PLANE_H
#define PLANE_H

class Plane : public Shape {

public:

    Plane(const Color&, float, const Vec3&, const Vec3&, const Vec3&, int, int);

    bool intersect(const Vec3&, const Vec3&, Vec3&) const;
    Vec3 normal(const Vec3&) const;

protected:

    Vec3 O;
    Vec3 e1;
    Vec3 e2;
    Vec3 n;
    int w;
    int h;
};

#endif
