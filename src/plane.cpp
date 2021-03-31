#include "plane.h"

Plane::Plane(const Color& col, float reflCoeff, const Vec3& O, const Vec3& e1, const Vec3& e2, int w, int h) :
    Shape(col, reflCoeff), O(O), e1(e1 * w), e2(e2 * h), w(w), h(h) {n = this->e1 ^ this->e2;}

bool Plane::intersect(const Vec3& p, const Vec3& dir, Vec3& intersection) const {

    // source : wikipedia - ray tracing - example
    float Ir, Ie1, Ie2, n_dot_dir = n * dir; Vec3 PO = (O - p);
    Ir = (n * PO) / n_dot_dir;
    Ie1 = ((e2 ^ PO) * dir) / n_dot_dir;
    Ie2 = ((PO ^ e1) * dir) / n_dot_dir;

    if(Ie1 >= 0 && Ie1 <= 1 && Ie2 >= 0 && Ie2 <= 1 && Ir >= 0 && (Ie1 + Ie2 <= 2)) {
        intersection.x = p.x + Ir * dir.x;
        intersection.y = p.y + Ir * dir.y;
        intersection.z = p.z + Ir * dir.z;
        return true;
    }

    return false;
}

Vec3 Plane::normal(const Vec3& v) const {
    return n;
}


