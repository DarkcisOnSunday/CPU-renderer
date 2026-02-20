#include "math/vec2.h"

Vec2::Vec2() : x(0), y(0) {}
Vec2::Vec2(float x_, float y_) : x(x_), y(y_) {}

Vec2 Vec2::operator+(const Vec2& other) const {
    return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator-(const Vec2& other) const {
    return Vec2(x - other.x, y - other.y);
}

Vec2 Vec2::operator*(float scalar) const {
    return Vec2(x * scalar, y * scalar);
}

Vec2 Vec2::operator/(float scalar) const {
    return Vec2(x / scalar, y / scalar);
}

float Vec2::Length() const {
    return std::sqrt(x*x + y*y);
}

float Vec2::LengthSquared() const {
    return x*x + y*y;
}

Vec2 Vec2::Normalized() const {
    float len = Length();
    if (len > 0.0f) return *this / len;
    return Vec2();
}

float Vec2::Dot(const Vec2& other) const {
    return x*other.x + y*other.y;
}

Vec2 Vec2::Zero() { return Vec2(0,0); }
Vec2 Vec2::One()  { return Vec2(1,1); }
