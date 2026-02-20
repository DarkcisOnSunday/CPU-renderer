#pragma once

#include "math/vec2.h"

#include <vector>
#include <cstdint>
#include <cmath>

struct Texture2D {
    int w = 0;
    int h = 0;
    std::vector<uint32_t> pixels; // AARRGGBB

    uint32_t SampleNearest(const Vec2& uv) const
    {
        if (w <= 0 || h <= 0 || pixels.empty()) return 0xFFFF00FF; // magenta debug

        // wrap to [0,1)
        float u = uv.x - std::floor(uv.x);
        float v = uv.y - std::floor(uv.y);

        int x = (int)(u * (float)w);
        int y = (int)(v * (float)h);

        if (x < 0) {x = 0;} if (x >= w) x = w - 1;
        if (y < 0) {y = 0;} if (y >= h) y = h - 1;

        return pixels[y * w + x];
    }

    static Texture2D MakeChecker(int w, int h, int cell = 32)
    {
        Texture2D t;
        t.w = w;
        t.h = h;
        t.pixels.resize(w * h);

        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                int cx = (x / cell) & 1;
                int cy = (y / cell) & 1;
                bool on = (cx ^ cy) != 0;

                // AARRGGBB
                t.pixels[y * w + x] = on ? 0xFFFFFFFF : 0xFF202020;
            }
        }
        return t;
    }
};
