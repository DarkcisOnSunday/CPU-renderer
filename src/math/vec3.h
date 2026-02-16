#pragma once

struct Vec3 {
    float x, y, z;
    
    Vec3();
    Vec3(float x, float y, float z);
    
    Vec3 operator+(const Vec3& other) const;
    Vec3 operator-(const Vec3& other) const;
    Vec3 operator*(float scalar) const;
    Vec3 operator/(float scalar) const;
    
    float Length() const;
    float LengthSquared() const;
    Vec3 Normalized() const;
    float Dot(const Vec3& other) const;
    Vec3 Cross(const Vec3& other) const;
    
    static Vec3 Zero();
    static Vec3 One();
    static Vec3 Up();      // (0, 1, 0)
    static Vec3 Down();    // (0, -1, 0)
    static Vec3 Left();    // (-1, 0, 0)
    static Vec3 Right();   // (1, 0, 0)
    static Vec3 Forward(); // (0, 0, -1) or (0, 0, 1)
    static Vec3 Back();    // (0, 0, 1) or (0, 0, -1)
};