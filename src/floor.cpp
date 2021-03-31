#include "floor.h"
#include <cmath>

Floor::Floor(const Color& col, float reflCoeff, const Vec3& O, const Vec3& e1, const Vec3& e2, int w, int h)
: Plane(col, reflCoeff, O, e1, e2, w, h) {}

Color Floor::getColor(const Vec3& p) const {
    float alpha = acos((p - O).angle(e1));
    float OPe1 = (p - O).length() * cos(alpha);
    float OPe2 = (p - O).length() * sin(alpha);
    if(((int)OPe1 / 200) % 2)
        if(((int)OPe2 / 200) % 2)
            return Color::white;
        else
            return Color::black;
    else
        if(((int)OPe2 / 200) % 2)
            return Color::black;
        else
            return Color::white;
}
