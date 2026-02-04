#include "math/vec4.h"
#include <cmath>

Vec4::Vec4() : x(0), y(0), z(0), w(1) {}
Vec4::Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
Vec4::Vec4(const Vec3& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}

Vec3 Vec4::XYZ() const {
    return Vec3(x, y, z);
}

Vec3 Vec4::ToVec3() const {
    if (w != 0) return Vec3(x/w, y/w, z/w);
    return Vec3(x, y, z);
}

Vec4 Vec4::operator+(const Vec4& other) const {
     return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
} 
 
Vec4 Vec4::operator-(const Vec4& other) const {
     return Vec4(x - other.x, y - other.y, z - other.z, w - other.w);
} 
 
Vec4 Vec4::operator*(float scalar) const {
     return Vec4(x * scalar, y * scalar, z * scalar, w * scalar);
} 
 
Vec4 Vec4::operator/(float scalar) const {
     return Vec4(x / scalar, y / scalar, z / scalar, w / scalar);
}

float Vec4::Length() const {
    return sqrtf(x*x + y*y + z*z + w*w);
}

float Vec4::LengthSquared() const {
    return x*x + y*y + z*z + w*w;
}

Vec4 Vec4::Normalized() const {
    float len = Length();
    if (len > 0) return *this / len;
    return Vec4();
}

float Vec4::Dot(const Vec4& other) const {
    return x*other.x + y*other.y + z*other.z + w*other.w;
}

Vec4 Vec4::Zero() { return Vec4(0, 0, 0, 1); }
Vec4 Vec4::One() { return Vec4(1, 1, 1, 1); }
Vec4 Vec4::Up() { return Vec4(0, 1, 0, 1); }
Vec4 Vec4::Down() { return Vec4(0, -1, 0, 1); }
Vec4 Vec4::Left() { return Vec4(-1, 0, 0, 1); }
Vec4 Vec4::Right() { return Vec4(1, 0, 0, 1); }
Vec4 Vec4::Forward() { return Vec4(0, 0, 1, 1); }  // Z-forward system
Vec4 Vec4::Back() { return Vec4(0, 0, -1, 1); }