#include "math/mat4.h"
#include <cstring>


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

Vec4 Mat4::operator*(const Vec4& v) const {
    return Vec4(
        m[0]*v.x + m[4]*v.y + m[8]*v.z + m[12]*v.w,
        m[1]*v.x + m[5]*v.y + m[9]*v.z + m[13]*v.w,
        m[2]*v.x + m[6]*v.y + m[10]*v.z + m[14]*v.w,
        m[3]*v.x + m[7]*v.y + m[11]*v.z + m[15]*v.w
    );
}