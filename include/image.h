#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <limits>
#include <functional>
#include <chrono>
#include <thread>

struct Vertex {
    int x, y;
    float z;
    uint32_t color;
};

class Image {
private:
    int width_;
    int height_;
    std::vector<uint32_t> buffer_;
    std::vector<float> zBuffer_;

    void* windowHandle_ = nullptr;
    bool realtime_mode_ = false;
    bool running_ = false;
    std::chrono::high_resolution_clock::time_point start_time_;
    
public:
    Image(int width, int height) 
        : width_(width), height_(height), 
          buffer_(width * height, 0xFF000000),
          zBuffer_(width * height, std::numeric_limits<float>::max())
    {
        start_time_ = std::chrono::high_resolution_clock::now();
    };
    
    int GetWidth() const { return width_; }
    
    int GetHeight() const { return height_; }

    uint32_t* GetBuffer() { return buffer_.data(); }

    float* GetZBuffer() { return zBuffer_.data(); }
    
    void Clear(uint32_t color) {
        std::fill(buffer_.begin(), buffer_.end(), color);
        ClearZBuffer();
    }

        void ClearZBuffer() {
        std::fill(zBuffer_.begin(), zBuffer_.end(), 
                  std::numeric_limits<float>::max());
    }
    
    void SetPixel(int x, int y, uint32_t color) {
        if (x >= 0 && x < width_ && y >= 0 && y < height_) {
            buffer_[y * width_ + x] = color;
        }
    }

    void SetPixel(int x, int y, float z, uint32_t color) {
        if (x >= 0 && x < width_ && y >= 0 && y < height_) {
            int index = y * width_ + x;
            if (z < zBuffer_[index]){
                zBuffer_[index] = z;
                buffer_[index] = color;
            }
        }
    }

    uint32_t GetPixel(int x, int y) const {
        if (x >= 0 && x < width_ && y >= 0 && y < height_) {
            return buffer_[y * width_ + x];
        }
        return 0;
    }

    void SetPixelUnsafe(int x, int y, uint32_t color) {
        buffer_[y * width_ + x] = color;
    }
    
    uint32_t GetPixelUnsafe(int x, int y) const {
        return buffer_[y * width_ + x];
    }
    
    bool IsInside(int x, int y) const {
        return x >= 0 && x < width_ && y >= 0 && y < height_;
    }

    void DrawLine(int x0, int y0, int x1, int y1, uint32_t color);

    void DrawHorizontalLine(int x0, int x1, int y, uint32_t color){
        if (x0 > x1) std::swap(x0, x1);
        for (int x = x0; x <= x1; x++) {
            SetPixel(x, y, color);
        }
    }

    void DrawVerticalLine(int y0, int y1, int x, uint32_t color){
        if (y0 > y1) std::swap(y0, y1);
        for (int y = y0; y <= y1; y++) {
            SetPixel(x, y, color);  
        }
    }

    void DrawHorizontalLine(int x0, int x1, float z0, float z1, int y, uint32_t color) {
        if (x0 > x1) {std::swap(x0, x1); std::swap(z0, z1);}

        if (x0 == x1) {
            SetPixel(x0, y, z0, color);
            return;
        }

        float dz = (z1 - z0) / (x1 - x0);
        float z = z0;
        for (int x = x0; x <= x1; x++) {
            SetPixel(x, y, z, color);
            z += dz;
        }
    }

    void DrawVerticalLine(int y0, int y1, float z0, float z1, int x, uint32_t color) {
        if (y0 > y1) {std::swap(y0, y1); std::swap(z0, z1);}

        if (y0 == y1) {
            SetPixel(x, y0, z0, color);
            return;
        }

        float dz = (z1 - z0) / (y1 - y0);
        float z = z0;
        for (int y = y0; y <= y1; y++) {
            SetPixel(x, y, z, color);
            z += dz;
        }
    }

    void DrawTriangle2D(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);

    void DrawTriangle3D(Vertex& v1, Vertex& v2, Vertex& v3);

    void DrawRect(int x, int y, int width, int height, uint32_t color);
    
    bool SaveToPPM(const std::string& filename) const;
    
    // Not Implemented Yet
    bool SaveToBMP(const std::string& filename) const;


    //Realtime part
    bool IsRunning() const { return running_; }
    void SetRunning(bool running) { running_ = running; }

    bool DrawToWindow(void* windowHandle);
    bool IsWindowActive() const;
    void ProcessEvents();
    void SetWindowHandle(void* handle) { windowHandle_ = handle; }
};