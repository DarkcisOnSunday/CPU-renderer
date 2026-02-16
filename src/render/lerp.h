#pragma once

#include "math/vec4.h"
#include "gfx/vertex.h"
#include "core/color.h"

#include <cstdint>
#include <algorithm>

float LerpFloat(float a, float b, float t) {
    return a + (b - a) * t;
}

static Vec4 LerpVec4(const Vec4& a, const Vec4& b, float t) {
    return a + (b - a) * t;
}

static VertexClip LerpVertexClip(const VertexClip& a, const VertexClip& b, float t)
{
    VertexClip out;
    out.clip  = LerpVec4(a.clip, b.clip, t);
    out.color = LerpVec4(a.color, b.color, t);
    return out;
}