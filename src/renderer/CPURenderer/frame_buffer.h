#pragma once

#include <vector>
#include <chrono>
#include <string>

class FrameBuffer {
private:
    int width_;
    int height_;
    std::vector<uint32_t> buffer_;
    std::vector<float> zBuffer_;
    
public:
    FrameBuffer(int width, int height) 
        : width_(width), height_(height), 
          buffer_(width * height, 0xFF000000),
          zBuffer_(width * height, 1.0f)
    {};
    
    int GetWidth() const { return width_; }
    
    int GetHeight() const { return height_; }

    uint32_t* GetBuffer() { return buffer_.data(); }

    float* GetZBuffer() { return zBuffer_.data(); }
    
    void Clear(const uint32_t& color = 0xFF000000) {
        std::fill(buffer_.begin(), buffer_.end(), color);
        ClearZBuffer();
    }

    void ClearZBuffer() {
        std::fill(zBuffer_.begin(), zBuffer_.end(), 1.0f);
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
};