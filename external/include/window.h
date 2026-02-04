#pragma once

#ifdef _WIN32
    #include <windows.h>
    HWND GetWindowHandle();
#endif

bool CreateAppWindow(const char* title, int width, int height);
bool ProcessAppWindowEvents();
void DestroyAppWindow();