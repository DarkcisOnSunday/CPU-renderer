#define _USE_MATH_DEFINES

#include "demo_scene.h"
#include "render/camera.h"
#include "core/color.h"
#include "core/mesh.h"
#include "core/scene.h"

#include <cmath>
#include <iostream>


static Mesh BuildCubeMesh()
{
    Mesh m;

    m.vertices = {
        {{ 0.25f,  0.25f,  0.25f}, RED},
        {{-0.25f,  0.25f,  0.25f}, GREEN},
        {{-0.25f, -0.25f,  0.25f}, BLUE},
        {{ 0.25f, -0.25f,  0.25f}, WHITE},

        {{ 0.25f,  0.25f, -0.25f}, YELLOW},
        {{-0.25f,  0.25f, -0.25f}, CYAN},
        {{-0.25f, -0.25f, -0.25f}, MAGENTA},
        {{ 0.25f, -0.25f, -0.25f}, ORANGE}
    };

    m.indices = {
        // front (z=+)
        0,1,2,  0,2,3,
        // back (z=-)
        4,6,5,  4,7,6,
        // left (x=-)
        1,5,6,  1,6,2,
        // right (x=+)
        0,3,7,  0,7,4,
        // top (y=+)
        0,4,5,  0,5,1,
        // bottom (y=-)
        3,2,6,  3,6,7
    };

    return m;
}

void UpdateScene(Scene& scene, double time) {
    scene.Clear();

    static Mesh cube = BuildCubeMesh();

    //float dz = 1.0f;
    //float angle = (float)(M_PI * time);

    RenderObject obj1;
    obj1.mesh = &cube;
    obj1.model =
        Mat4::Translation(0,0,1.0f);
    scene.objects.push_back(obj1);

    RenderObject obj2;
    obj2.mesh = &cube;
    obj2.model =
        Mat4::Translation(1.3f, 0.4f, 2.0f) *
        Mat4::RotationY((float)time * 0.7f);
    scene.objects.push_back(obj2);
}