#pragma once

#include "core/color.h"

#include <math/vec4.h>
#include <core/texture2D.h>

enum class TextureMode { None, Modulate, Replace };

struct Material {
    const Texture2D* albedo = nullptr;
    TextureMode mode = TextureMode::Modulate;

    Vec4 Eval(const Vec2& uv, const Vec4& vtxColor) const
    {
        Vec4 out;
        Vec4 tex = Vec4::One();

        if (albedo) {tex = UnpackColor(albedo->SampleNearest(uv));}

        switch (mode)
        {
        case TextureMode::Replace:
            out = tex;
            break;
        case TextureMode::Modulate:
            out = Vec4(tex.x*vtxColor.x,
                        tex.y*vtxColor.y,
                        tex.z*vtxColor.z,
                        tex.w*vtxColor.w);
            break;
        case TextureMode::None:
            out = vtxColor;
        default:
            break;
        }

        return out;
    }

    uint32_t EvalPacked(const Vec2& uv, const Vec4& vtxColor) const
    {
        return PackColor(Eval(uv, vtxColor));
    }
};