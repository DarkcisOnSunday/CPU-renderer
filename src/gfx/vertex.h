#pragma once

#include "math/vec3.h"
#include "math/vec4.h"

#include <cstdint>

struct VertexScreen {
    int x, y;
    float z;
    float invW;
    Vec4 color;
};

struct VertexClip {
    Vec4 clip;
    Vec4 color;
};

struct Vertex3D {
    Vec3 pos;
    uint32_t color;
};