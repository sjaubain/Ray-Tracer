#include <iostream>

#ifndef VEC_H
#define VEC_H

class Vec3 {

public:

    Vec3();
    Vec3(float, float, float);
    float length() const;
    Vec3 normalized() const;
    void print() const;
    float angle(const Vec3&) const;

    Vec3 operator + (const Vec3&) const;
    Vec3 operator - (const Vec3&) const;
    Vec3 operator * (float) const;
    Vec3 operator ^ (const Vec3&) const;
    friend Vec3 operator * (float, const Vec3&); // in order to use lambda as left operand
    float operator * (const Vec3&) const;
    Vec3& rotateZ(float);

public:

    float x; float y; float z;
};

#endif
