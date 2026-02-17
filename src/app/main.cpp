#define _USE_MATH_DEFINES

#include "DcisML/include/window.h"

#include "core/scene.h"
#include "gfx/frame_buffer.h"
#include "gfx/rasterizer.h"
#include "render/renderer.h"
#include "render/camera.h"
#include "app/demo_scene.h"

#include <cmath>


int main()
{
    const int width = 800;
    const int height = 800;

    auto window = dcis::AppWindow::CreateAppWindow("SoftRender Demo", width, height);
    if (!window.IsValid())
        return 1;

    Scene scene;
    FrameBuffer fb(width, height);
    Rasterizer rast;
    Renderer renderer;
    Camera cam;

    cam.SetPerspective(70.0f * (float)M_PI / 180.0f, (float)width / (float)height, 0.05f, 40.0f);

    renderer.Resize(width, height);
    renderer.SetCamera(&cam);

    dcis::AppTimePoint start = dcis::GetAppTimeNow();

    while (window.ProcessAppWindowEvents())
    {
        double t = dcis::GetAppTimeSecondsBetween(start, dcis::GetAppTimeNow());
        fb.Clear();

        UpdateScene(scene, t);
        renderer.Render(fb, rast, scene);
        window.PresentAppWindow(fb.GetBuffer(), fb.GetWidth(), fb.GetHeight());

        // -------- camera controls --------
        const float moveSpeed = 1.5f;      // units/sec
        const float mouseSens = 0.0025f;   // rad per pixel

        // dt (у тебя Sleep 16ms, но лучше считать)
        static double prevT = t;
        float dt = (float)(t - prevT);
        prevT = t;
        if (dt > 0.1f) dt = 0.1f;

        // look (hold RMB)
        if (window.IsAppMouseDown(dcis::AppMouseButton::Right)) {
            int dx, dy;
            window.GetAppMouseDelta(dx, dy);

            cam.AddYawPitch((float)dx * mouseSens, (float)dy * mouseSens);
        }

        // movement
        Vec3 move(0,0,0);
        if (window.IsAppKeyDown('A')) move.x -= 1.0f;
        if (window.IsAppKeyDown('D')) move.x += 1.0f;
        if (window.IsAppKeyDown('W')) move.z += 1.0f;
        if (window.IsAppKeyDown('S')) move.z -= 1.0f;
        if (window.IsAppKeyDown(0x20)) move.y += 1.0f;
        if (window.IsAppKeyDown(0x11)) move.y -= 1.0f;

        if (move.LengthSquared() > 0.0f) {
            move = move.Normalized() * (moveSpeed * dt);
            cam.MoveLocal(move);
        }

        cam.UpdateView();

        dcis::SleepAppMs(16);
    }

    window.DestroyAppWindow();
    return 0;
}