#pragma once

#include <cstdint>

struct Camera;
struct Scene;

struct FrameContext {
    float dt;
    double time;
    uint32_t width;
    uint32_t height;
};

class IRenderer {
public:
    virtual ~IRenderer() = default;
    virtual void OnResize(uint32_t w, uint32_t h) = 0;

    virtual void BeginFrame(const FrameContext& ctx) = 0;
    virtual void RenderScene(const Scene& scene, const Camera& cam) = 0;
    virtual void EndFrame() = 0;
};