#include <iostream>
#include "shape.h"
#include "plane.h"

#ifndef FLOOR_H
#define FLOOR_H

class Floor : public Plane {

public:

    Floor(const Color&, float, float, const Vec3&, const Vec3&, const Vec3&, int, int);
    Color getColor(const Vec3&) const;
    void print() const;
};

#endif
