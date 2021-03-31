#include "vec3.h"
#include <cmath>

Vec3::Vec3() : x(0), y(0), z(0) {}

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

float Vec3::length() const {return sqrt(x * x + y * y + z * z);}

Vec3 Vec3::normalized() const {return 1 / length() * *this;}

Vec3 Vec3::operator + (const Vec3& v) const {return Vec3(x + v.x, y + v.y, z + v.z);}

Vec3 Vec3::operator - (const Vec3& v) const {return Vec3(x - v.x, y - v.y, z - v.z);}

Vec3 Vec3::operator * (float lambda) const {return Vec3(lambda * x, lambda * y, lambda * z);}

Vec3 Vec3::operator ^ (const Vec3& v) const {return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);}

Vec3 operator * (float lambda, const Vec3& v) {return v * lambda;}

float Vec3::operator * (const Vec3& v) const {return x * v.x + y * v.y + z * v.z;}

void Vec3::print() const {std::cout << "(" << x << "," << y << "," << z << ")" << std::endl;}

float Vec3::angle(const Vec3& v) const {return (*this * v)/(this->length() * v.length());}

Vec3& Vec3::rotateZ(float theta) {this->x = cos(theta) * this->x - sin(theta) * this->y;
                                  this->y = sin(theta) * this->x + cos(theta) * this->y;
                                  return *this;}
