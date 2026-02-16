#pragma once

#include "math/vec3.h"
#include "render/transform.h"

#include <cstdint>
#include <vector>

struct DrawLineCmd {
    Transform model;
    Vec3 a;
    Vec3 b;
    uint32_t colorA;
    uint32_t colorB;
};

struct DrawTriCmd {
    Transform model;
    Vec3 a, b, c;
    uint32_t colorA, colorB, colorC;
};

struct DrawList {
    std::vector<DrawLineCmd> lines;
    std::vector<DrawTriCmd>  tris;

    void Clear() { lines.clear(); tris.clear(); }
};
