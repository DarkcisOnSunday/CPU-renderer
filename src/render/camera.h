#pragma once

#include "math/vec3.h"
#include "math/vec4.h"
#include "math/mat4.h"

#include <cmath>

constexpr float PI = 3.14159265358979323846f;

struct Camera {
    Vec3 pos {0,0,0};

    float yaw = 0.0f;
    float pitch = 0.0f;

    float fov_rad = 70.0f * PI / 180.0f;
    float aspect = 1.0f;
    float z_near = 0.05f;
    float z_far = 40.0f;

    Mat4 view = Mat4::Identity();
    Mat4 proj = Mat4::Identity();

    void SetPerspective(float fov, float asp, float nz, float fz) {
        fov_rad = fov; aspect = asp; z_near = nz; z_far = fz;
        proj = Mat4::Perspective(fov_rad, aspect, z_near, z_far);
    }

    Vec3 Forward() const {
        float cy = std::cos(yaw),  sy = std::sin(yaw);
        float cp = std::cos(pitch),sp = std::sin(pitch);
        return Vec3(sy * cp, -sp, cy * cp);
    }

    Vec3 Right() const {
        return (Vec3::Up() ^ Forward()).Normalized();
    }

    void AddYawPitch(float dyaw, float dpitch) {
        yaw += dyaw;
        pitch += dpitch;

        const float limit = (PI / 2.0f) - 1e-8f; //~1.55334306f
        if (pitch >  limit) pitch =  limit;
        if (pitch < -limit) pitch = -limit;
    }

    void MoveLocal(const Vec3& delta) {
        Vec3 f = Forward();
        Vec3 r = Right();
        Vec3 u = Vec3::Up();
        pos = pos + r * delta.x + u * delta.y + f * delta.z;
    }

    void UpdateView() {
        Vec3 f = Forward();
        Vec3 target = pos + f;
        view = Mat4::LookAt(
            Vec4(pos.x, pos.y, pos.z, 1.0f),
            Vec4(target.x, target.y, target.z, 1.0f),
            Vec4(0,1,0,0)
        );
    }
};