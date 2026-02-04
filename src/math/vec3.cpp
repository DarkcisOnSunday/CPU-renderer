#include "math/vec3.h"
#include <cmath>

Vec3::Vec3() : x(0), y(0), z(0) {}
Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

Vec3 Vec3::operator+(const Vec3& other) const {
    return Vec3(x + other.x, y + other.y, z + other.z);
}

Vec3 Vec3::operator-(const Vec3& other) const {
    return Vec3(x - other.x, y - other.y, z - other.z);
}

Vec3 Vec3::operator*(float scalar) const {
    return Vec3(x * scalar, y * scalar, z * scalar);
}

Vec3 Vec3::operator/(float scalar) const {
    return Vec3(x / scalar, y / scalar, z / scalar);
}

float Vec3::Length() const {
    return sqrtf(x*x + y*y + z*z);
}

float Vec3::LengthSquared() const {
    return x*x + y*y + z*z;
}

Vec3 Vec3::Normalized() const {
    float len = Length();
    if (len > 0) return *this / len;
    return Vec3();
}

float Vec3::Dot(const Vec3& other) const {
    return x*other.x + y*other.y + z*other.z;
}

Vec3 Vec3::Cross(const Vec3& other) const {
    return Vec3(
        y*other.z - z*other.y,
        z*other.x - x*other.z,
        x*other.y - y*other.x
    );
}

Vec3 Vec3::Zero() { return Vec3(0, 0, 0); }
Vec3 Vec3::One() { return Vec3(1, 1, 1); }
Vec3 Vec3::Up() { return Vec3(0, 1, 0); }
Vec3 Vec3::Down() { return Vec3(0, -1, 0); }
Vec3 Vec3::Left() { return Vec3(-1, 0, 0); }
Vec3 Vec3::Right() { return Vec3(1, 0, 0); }
Vec3 Vec3::Forward() { return Vec3(0, 0, 1); }  // Z-forward system
Vec3 Vec3::Back() { return Vec3(0, 0, -1); }