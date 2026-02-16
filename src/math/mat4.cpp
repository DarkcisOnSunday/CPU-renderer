#include "math/mat4.h"
#include "math/vec4.h"
#include "math/vec3.h"

#include <cstring>
#include <cmath>

Mat4::Mat4() {
    memset(m, 0, sizeof(m));
}

Mat4::Mat4(const float* data) {
    memcpy(m, data, sizeof(m));
}

//column-major!
float& Mat4::operator()(int row, int col) {
    return m[col * 4 + row];
}

float Mat4::operator()(int row, int col) const {
    return m[col * 4 + row];
}

Mat4 Mat4::Identity() {
    Mat4 result;
    result(0,0) = 1; result(1,1) = 1; result(2,2) = 1; result(3,3) = 1;
    return result;
}

Mat4 Mat4::operator*(const Mat4& other) const {
    Mat4 r;

    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            float sum = 0.0f;
            for (int k = 0; k < 4; k++) {
                sum += (*this)(row, k) * other(k, col);
            }
            r(row, col) = sum;
        }
    }

    return r;
}

Vec4 Mat4::operator*(const Vec4& v) const {
    return Vec4(
        m[0]*v.x + m[4]*v.y + m[8]*v.z + m[12]*v.w,
        m[1]*v.x + m[5]*v.y + m[9]*v.z + m[13]*v.w,
        m[2]*v.x + m[6]*v.y + m[10]*v.z + m[14]*v.w,
        m[3]*v.x + m[7]*v.y + m[11]*v.z + m[15]*v.w
    );
}

Mat4 Mat4::Translation(float x, float y, float z) {
    Mat4 r = Mat4::Identity();
    r(0,3) = x;
    r(1,3) = y;
    r(2,3) = z;
    return r;
}

Mat4 Mat4::Translation(const Vec4& v) {
    return Translation(v.x, v.y, v.z);
}

Mat4 Mat4::RotationX(float angleRad) {
    Mat4 r = Mat4::Identity();
    float c = std::cos(angleRad);
    float s = std::sin(angleRad);

    r(1,1) =  c;
    r(1,2) = -s;
    r(2,1) =  s;
    r(2,2) =  c;

    return r;
}

Mat4 Mat4::RotationY(float angleRad) {
    Mat4 r = Mat4::Identity();
    float c = std::cos(angleRad);
    float s = std::sin(angleRad);

    r(0,0) =  c;
    r(0,2) =  s;
    r(2,0) = -s;
    r(2,2) =  c;

    return r;
}

Mat4 Mat4::Perspective(float fovRad, float aspect, float near, float far)
{
    Mat4 r;
    float f = 1.0f / std::tan(fovRad * 0.5f);

    r(0,0) = f / aspect;
    r(1,1) = f;

    r(2,2) = far / (far - near);
    r(2,3) = (-near * far) / (far - near);

    r(3,2) = 1.0f;
    r(3,3) = 0.0f;

    return r;
}

Mat4 Mat4::LookAt(const Vec4& eye4, const Vec4& target4, const Vec4& up4)
{
    Vec3 eye(eye4.x, eye4.y, eye4.z);
    Vec3 target(target4.x, target4.y, target4.z);
    Vec3 up(up4.x, up4.y, up4.z);

    // LH: forward +Z
    Vec3 f = (target - eye).Normalized();
    Vec3 r = up.Cross(f).Normalized();
    Vec3 u = f.Cross(r);

    Mat4 m = Mat4::Identity();

    m(0,0) = r.x;  m(0,1) = r.y;  m(0,2) = r.z;
    m(1,0) = u.x;  m(1,1) = u.y;  m(1,2) = u.z;
    m(2,0) = f.x;  m(2,1) = f.y;  m(2,2) = f.z;

    m(0,3) = -r.Dot(eye);
    m(1,3) = -u.Dot(eye);
    m(2,3) = -f.Dot(eye);

    return m;
}