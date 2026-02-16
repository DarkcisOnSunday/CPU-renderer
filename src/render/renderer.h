#pragma once

#include "math/mat4.h"
#include "render/viewport.h"
#include "render/camera.h"
#include "core/scene.h"

class FrameBuffer;
class Rasterizer;

class Renderer {
public:
    Viewport vp;
    const Camera* camera = nullptr; 

    void Resize(int w, int h) { vp.width = w; vp.height = h; }
    void SetCamera(const Camera* cam) { camera = cam;}

    void Render(FrameBuffer& image, Rasterizer& rast, const Scene& scene);
};