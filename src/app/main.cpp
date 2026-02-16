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

        UpdateScene(scene, cam, t);
        renderer.Render(fb, rast, scene);
        window.PresentAppWindow(fb.GetBuffer(), fb.GetWidth(), fb.GetHeight());
        
        dcis::SleepAppMs(16);
    }

    window.DestroyAppWindow();
    return 0;
}