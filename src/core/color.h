#pragma once

#include "math/vec4.h"

#include <cstdint>

static uint32_t MakeColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
    return (a << 24) | (r << 16) | (g << 8) | b;
}

static const uint32_t WHITE = MakeColor(255, 255, 255);
static const uint32_t BLACK = MakeColor(0, 0, 0);
static const uint32_t RED = MakeColor(255, 0, 0);
static const uint32_t GREEN = MakeColor(0, 255, 0);
static const uint32_t BLUE = MakeColor(0, 0, 255);
static const uint32_t YELLOW = MakeColor(255,255,0);
static const uint32_t CYAN = MakeColor(0,255,255);
static const uint32_t MAGENTA = MakeColor(255,0,255);
static const uint32_t ORANGE = MakeColor(255,128,0);

inline Vec4 UnpackColor(uint32_t color) {
    float a = ((color >> 24) & 255) / 255.0f;
    float r = ((color >> 16) & 255) / 255.0f;
    float g = ((color >> 8)  & 255) / 255.0f;
    float b = ((color >> 0)  & 255) / 255.0f;

    return Vec4(a, r, g, b);
}

inline uint32_t PackColor(const Vec4& v) {
    auto to_u8 = [](float x) -> uint32_t {
        if (x < 0.0f) x = 0.0f;
        if (x > 1.0f) x = 1.0f;
        return (uint32_t)(x * 255.0f + 0.5f);
    };
    uint32_t a = to_u8(v.x);
    uint32_t r = to_u8(v.y);
    uint32_t g = to_u8(v.z);
    uint32_t b = to_u8(v.w);
    return (a<<24) | (r<<16) | (g<<8) | b;
}