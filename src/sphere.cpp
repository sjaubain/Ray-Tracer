#include "sphere.h"
#include <cmath>

Sphere::Sphere(const Color& col, float reflCoeff, float opacity, const Vec3& center, float radius)
: Shape(col, reflCoeff, opacity), center(center), radius(radius) {}

/**
 * p   : a point belonging to the line
 * dir : the orientation vector of the line
 * parametric equation of the line : p + dir * t, t in R
 */
bool Sphere::intersect(const Vec3& p, const Vec3& dir, Vec3& intersection) const {

    // if distance between line and center < radius => intersection(s)
    if(((center - p) ^ dir).length() / dir.length() < radius && dir.angle(center - p) > 0) {

        float p_0 = p.x, p_1 = p.y, p_2 = p.z;
        float v_0 = dir.x, v_1 = dir.y, v_2 = dir.z;
        float c_0 = center.x, c_1 = center.y, c_2 = center.z;

        float a = v_0 * v_0 + v_1 * v_1 + v_2 * v_2;
        float b = 2 * (v_0 * (p_0 - c_0) + v_1 * (p_1 - c_1) + v_2 * (p_2 - c_2));
        float c = (p_0 - c_0) * (p_0 - c_0) + (p_1 - c_1) * (p_1 - c_1) + (p_2 - c_2) * (p_2 - c_2) - radius * radius;

        float det = sqrt(b * b - 4 * a * c);
        float t = (p - center).length() > radius ? (-1 * b - det) / (2 * a) : (-1 * b + det) / (2 * a);
        intersection.x = p_0 + t * v_0; intersection.y = p_1 + t * v_1; intersection.z = p_2 + t * v_2;
        return true;
    }

    return false;
}

Vec3 Sphere::normal(const Vec3& v) const {
    return v - center;
}
