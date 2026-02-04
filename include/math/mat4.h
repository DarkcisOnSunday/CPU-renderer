#pragma once
#include "vec4.h"

struct Mat4 {
    // Column-major: m[col * 4 + row]
    float m[16];
    
    Mat4();
    Mat4(const float* data);
    
    float& operator()(int row, int col);
    float operator()(int row, int col) const;
    
    Mat4 operator*(const Mat4& other) const;
    Vec4 operator*(const Vec4& v) const;
    
    Mat4 Transposed() const;
    
    static Mat4 Identity();
    static Mat4 Translation(float x, float y, float z);
    static Mat4 Translation(const Vec4& v);
    static Mat4 Scale(float x, float y, float z);
    static Mat4 Scale(float uniform);
    static Mat4 RotationX(float angleRad);
    static Mat4 RotationY(float angleRad);
    static Mat4 RotationZ(float angleRad);
    static Mat4 Perspective(float fovRad, float aspect, float near, float far);
    static Mat4 Orthographic(float left, float right, 
                             float bottom, float top, 
                             float near, float far);
    
    static Mat4 LookAt(const Vec4& eye,
                       const Vec4& target,
                       const Vec4& up);
};