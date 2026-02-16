#pragma once

struct Vec3;

struct Vec4 {
    float x, y, z, w;
    
    Vec4();
    Vec4(float x, float y, float z, float w);
    Vec4(const Vec3& v, float w = 1.0f);
    
    Vec3 XYZ() const;
    Vec3 ToVec3() const;
    
    Vec4 operator+(const Vec4& other) const;
    Vec4 operator-(const Vec4& other) const;
    Vec4 operator*(float scalar) const;
    Vec4 operator/(float scalar) const;
    
    float Length() const;
    float LengthSquared() const;
    Vec4 Normalized() const;
    float Dot(const Vec4& other) const;
    Vec4 Cross(const Vec4& other) const;
    
    static Vec4 Zero();
    static Vec4 One();
    static Vec4 Up();      // (0, 1, 0)
    static Vec4 Down();    // (0, -1, 0)
    static Vec4 Left();    // (-1, 0, 0)
    static Vec4 Right();   // (1, 0, 0)
    static Vec4 Forward(); // (0, 0, -1) or (0, 0, 1)
    static Vec4 Back();    // (0, 0, 1) or (0, 0, -1)
};