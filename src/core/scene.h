#pragma once

#include "math/mat4.h"
#include "core/mesh.h"

#include <vector>

struct RenderObject {
    const Mesh* mesh = nullptr;
    Mat4 model = Mat4::Identity();
};

struct Scene {
    std::vector<RenderObject> objects;
    void Clear() { objects.clear(); }
};