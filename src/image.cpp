#include "image.h"
#include <fstream>
#include <iostream>
#include <cmath>

#ifdef _WIN32
#include <windows.h>
#else
// Linux
#endif

bool Image::SaveToPPM(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return false;
    }
    
    // Заголовок PPM (P6 - binary RGB)
    file << "P6\n" << width_ << " " << height_ << "\n255\n";
    
    // Пишем пиксели (PPM ожидает RGB, у нас RGBA)
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            uint32_t pixel = buffer_[y * width_ + x];
            
            // Извлекаем компоненты (RGBA)
            uint8_t r = (pixel >> 16) & 0xFF;
            uint8_t g = (pixel >> 8) & 0xFF;
            uint8_t b = pixel & 0xFF;
            
            file.put(r);
            file.put(g);
            file.put(b);
        }
    }
    
    file.close();
    std::cout << "Saved to " << filename << std::endl;
    return true;
}

void Image::DrawLine(int x0, int y0, int x1, int y1, uint32_t color)
{
    if (x0 == x1)
        DrawVerticalLine(y0, y1, x0, color);
    if (y0 == y1)
        DrawHorizontalLine(x0, x1, y0, color);

    int steep = abs(y1 - y0) > abs(x1 - x0);

    if (steep)
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = abs(y1 - y0);
    int error = dx / 2;
    int ystep = (y0 < y1) ? 1 : -1;
    int y = y0;

    for (int x = x0; x <= x1; x++)
    {
        SetPixel(steep ? y : x, steep ? x : y, color);
        error -= dy;
        if (error < 0)
        {
            y += ystep;
            error += dx;
        }
    }
}

void Image::DrawTriangle2D(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color)
{
    if (y1 > y2) { std::swap(x1, x2); std::swap(y1, y2); }
    if (y1 > y3) { std::swap(x1, x3); std::swap(y1, y3); }
    if (y2 > y3) { std::swap(x2, x3); std::swap(y2, y3); }
    
    float dx_left1 = (float)(x2 - x1) / (y2 - y1);
    float dx_left2 = (float)(x3 - x1) / (y3 - y1);
    float dx_right1 = (float)(x3 - x2) / (y3 - y2);
    
    float x_left = x1;
    float x_right = x1;
    
    for (int y = y1; y <= y2; y++) {
        DrawHorizontalLine((int)x_left, (int)x_right, y, color);
        x_left += dx_left1;
        x_right += dx_left2;
    }
    
    x_left = x2;
    
    for (int y = y2; y <= y3; y++) {
        DrawHorizontalLine((int)x_left, (int)x_right, y, color);
        x_left += dx_right1;
        x_right += dx_left2;
    }
}

void Image::DrawTriangle3D(Vertex& v1, Vertex& v2, Vertex& v3)
{
    uint32_t color;
    if (v1.color == v2.color && v1.color == v3.color)
        color = v1.color;

    if (v1.y > v2.y) { std::swap(v1, v2); }
    if (v1.y > v3.y) { std::swap(v1, v3); }
    if (v2.y > v3.y) { std::swap(v2, v3); }

    float dx_left1 = (float)(v2.x - v1.x) / (v2.y - v1.y);
    float dx_left2 = (float)(v3.x - v1.x) / (v3.y - v1.y);
    float dx_right1 = (float)(v3.x - v2.x) / (v3.y - v2.y);
    
    float dz_left1 = (float)(v2.z - v1.z) / (v2.y - v1.y);
    float dz_left2 = (float)(v3.z - v1.z) / (v3.y - v1.y);
    float dz_right1 = (float)(v3.z - v2.z) / (v3.y - v2.y);
    
    float x_left = v1.x;
    float x_right = v1.x;
    float z_left = v1.z;
    float z_right = v1.z;
    
    for (int y = v1.y; y <= v2.y; y++) {
        DrawHorizontalLine((int)x_left, (int)x_right, z_left, z_right, y, color);
        x_left += dx_left1;
        x_right += dx_left2;
        z_left += dz_left1;
        z_right += dz_left2;
    }
    
    x_left = v2.x;
    z_left = v2.z;
    
    for (int y = v2.y; y <= v3.y; y++) {
        DrawHorizontalLine((int)x_left, (int)x_right, z_left, z_right, y, color);
        x_left += dx_right1;
        x_right += dx_left2;
        z_left += dz_right1;
        z_right += dz_left2;
    }
}

#ifdef _WIN32

bool Image::DrawToWindow(void* windowHandle) {
    if (!windowHandle) return false;
    
    HWND hwnd = static_cast<HWND>(windowHandle);
    HDC hdc = GetDC(hwnd);
    
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    int clientWidth = clientRect.right - clientRect.left;
    int clientHeight = clientRect.bottom - clientRect.top;
    
    HDC memDC = CreateCompatibleDC(hdc);
    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width_;
    bmi.bmiHeader.biHeight = -height_;  // Top-down
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    
    void* bits = nullptr;
    HBITMAP hbitmap = CreateDIBSection(
        hdc, &bmi, DIB_RGB_COLORS, &bits, nullptr, 0
    );
    
    if (!hbitmap) {
        DeleteDC(memDC);
        ReleaseDC(hwnd, hdc);
        return false;
    }
    
    memcpy(bits, buffer_.data(), width_ * height_ * 4);
    
    HGDIOBJ oldBitmap = SelectObject(memDC, hbitmap);
    
    StretchBlt(hdc, 0, 0, clientWidth, clientHeight,memDC, 0, 0, width_, height_, SRCCOPY);
    
    SelectObject(memDC, oldBitmap);
    DeleteObject(hbitmap);
    DeleteDC(memDC);
    ReleaseDC(hwnd, hdc);
    
    return true;
}

#else
bool Image::DrawToWindow(void* windowHandle) {
    // Linux
    return false;
}
#endif

bool Image::IsWindowActive() const {
#ifdef _WIN32
    if (!windowHandle_) return false;
    HWND hwnd = static_cast<HWND>(windowHandle_);
    return IsWindow(hwnd) && IsWindowVisible(hwnd);
#else
    return false;
#endif
}

void Image::ProcessEvents() {
#ifdef _WIN32
    if (!windowHandle_) return;
    
    HWND hwnd = static_cast<HWND>(windowHandle_);
    MSG msg;
    while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        
        if (msg.message == WM_CLOSE || msg.message == WM_QUIT) {
            running_ = false;
        }
    }
}
#endif

void Image::DrawRect(int x, int y, int width, int height, uint32_t color) {
    DrawTriangle2D(x, y, x + width, y, x, y + height, color);
    DrawTriangle2D(x, y + height, x + width, y, x + width, y + height, color);
}