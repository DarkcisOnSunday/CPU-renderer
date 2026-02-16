#include "render/renderer.h"
#include "gfx/frame_buffer.h"
#include "gfx/rasterizer.h"
#include "render/render_utils.h"
#include "render/camera.h"

#include <vector>

static inline bool BackfaceCulling(const VertexScreen& a, const VertexScreen& b, const VertexScreen& c) {
    float s = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    return s <= 0;
}

inline float near(const VertexClip& v){ return v.clip.z;}
inline float far(const VertexClip& v){ return v.clip.w - v.clip.z;}
inline float left(const VertexClip& v){ return v.clip.w + v.clip.x;}
inline float right(const VertexClip& v){ return v.clip.w - v.clip.x;}
inline float bottom(const VertexClip& v){ return v.clip.w + v.clip.y;}
inline float top(const VertexClip& v){ return v.clip.w - v.clip.y;}

void Renderer::Render(FrameBuffer& image, Rasterizer& rast, const Scene& scene)
{
    for (const auto& obj : scene.objects) {
        if (!obj.mesh) continue;

        Mat4 MVP = camera->proj * (camera->view * obj.model);

        const auto& verts = obj.mesh->vertices;
        const auto& idx   = obj.mesh->indices;

        for (size_t i = 0; i + 2 < idx.size(); i += 3) {
            const std::vector<Vertex3D> poly = {verts[idx[i+0]], verts[idx[i+1]], verts[idx[i+2]]};

            std::vector<VertexClip> clipVertex;
            ModelToClipVertex(poly, MVP, clipVertex);

            ClipByPlane(clipVertex, near);
            ClipByPlane(clipVertex, far);
            ClipByPlane(clipVertex, left);
            ClipByPlane(clipVertex, right);
            ClipByPlane(clipVertex, bottom);
            ClipByPlane(clipVertex, top);

            if (clipVertex.size() == 0) continue;

            for (size_t j = 1; j + 1 < clipVertex.size(); j++) {
                VertexScreen a,b,c;
                if (!ClipToScreenVertex(clipVertex[0], vp, a)) continue;
                if (!ClipToScreenVertex(clipVertex[j], vp, b)) continue;
                if (!ClipToScreenVertex(clipVertex[j+1], vp, c)) continue;

                //if (BackfaceCulling(a,b,c)) continue;

                rast.DrawTriangle3D(image, a,b,c);
            }
        }
    }
}

