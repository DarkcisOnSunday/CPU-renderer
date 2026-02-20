#pragma once

#include <cmath>

struct Vec2 {
    float x, y;

    Vec2();
    Vec2(float x, float y);

    Vec2 operator+(const Vec2& other) const;
    Vec2 operator-(const Vec2& other) const;
    Vec2 operator*(float scalar) const;
    Vec2 operator/(float scalar) const;

    float Length() const;
    float LengthSquared() const;

    Vec2 Normalized() const;

    float Dot(const Vec2& other) const;

    static Vec2 Zero();
    static Vec2 One();
};