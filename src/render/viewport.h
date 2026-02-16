#pragma once

#include "math/vec3.h"
#include "gfx/vertex.h"
#include "core/color.h"

struct Viewport {
    int x = 0;
    int y = 0;
    int width = 1;
    int height = 1;

    VertexScreen ToScreen(const Vec3& ndc, uint32_t color) const {
        VertexScreen out{};
        out.x = (int)((ndc.x + 1.0f) * 0.5f * (float)width);
        out.y = (int)((1.0f - (ndc.y + 1.0f) * 0.5f) * (float)height);
        out.z = ndc.z;
        out.color = UnpackColor(color);
        return out;
    }
};