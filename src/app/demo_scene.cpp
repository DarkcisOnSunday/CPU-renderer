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

void UpdateScene(Scene& scene, Camera& cam, double time) {
    scene.Clear();

    static Mesh cube = BuildCubeMesh();

    float dz = 1.0f;
    float angle = (float)(M_PI * time);

    RenderObject obj;
    obj.mesh = &cube;
    obj.model =
        Mat4::Translation(0,0,dz) *
        Mat4::RotationY(angle) *
        Mat4::RotationX(30.0f * M_PI / 180.0f);

    scene.objects.push_back(obj);

    Vec3 center(obj.model(0,3), obj.model(1,3), obj.model(2,3));

    float radius = 2.5f;
    float yaw = (float)time * 0.8f;
    float pitch = 0.35f;

    cam.OrbitAround(center, radius, yaw, pitch);
}