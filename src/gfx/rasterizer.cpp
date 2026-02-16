#include "rasterizer.h"
#include "frame_buffer.h"
#include "core/color.h"

#include <cmath>
#include <iostream>

void Rasterizer::DrawLine2D(FrameBuffer& image, int x0, int y0, int x1, int y1, uint32_t color)
{
    if (x0 == x1) {
        DrawVerticalLine(image, y0, y1, x0, color);
        return;
    }
    if (y0 == y1) {
        DrawHorizontalLine(image, x0, x1, y0, color);
        return;
    }

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
        image.SetPixel(steep ? y : x, steep ? x : y, color);
        error -= dy;
        if (error < 0)
        {
            y += ystep;
            error += dx;
        }
    }
}

void Rasterizer::DrawLine3D(FrameBuffer& image, const VertexScreen a, const VertexScreen b)
{
    float x0 = (float)a.x, y0 = (float)a.y, z0 = a.z;
    float x1 = (float)b.x, y1 = (float)b.y, z1 = b.z;

    float dx = x1 - x0;
    float dy = y1 - y0;

    float steps = std::fabs(dx) > std::fabs(dy) ? std::fabs(dx) : std::fabs(dy);
    if (steps < 1.0f) {
        image.SetPixel((int)x0, (int)y0, z0, PackColor(a.color));
        return;
    }

    float xInc = dx / steps;
    float yInc = dy / steps;
    float zInc = (z1 - z0) / steps;

    float x = x0, y = y0, z = z0;
    for (int i = 0; i <= (int)steps; i++) {
        image.SetPixel((int)x, (int)y, z, PackColor(a.color));
        x += xInc;
        y += yInc;
        z += zInc;
    }
}

void Rasterizer::DrawHorizontalLine(FrameBuffer& image, int x0, int x1, int y, uint32_t color){
    if (x0 > x1) std::swap(x0, x1);
    for (int x = x0; x <= x1; x++) {
        image.SetPixel(x, y, color);
    }
}

void Rasterizer::DrawVerticalLine(FrameBuffer& image, int y0, int y1, int x, uint32_t color){
    if (y0 > y1) std::swap(y0, y1);
    for (int y = y0; y <= y1; y++) {
        image.SetPixel(x, y, color);  
    }
}

void Rasterizer::DrawHorizontalLine(FrameBuffer& image, int x0, int x1, float z0, float z1, int y, uint32_t color) {
    if (x0 > x1) {std::swap(x0, x1); std::swap(z0, z1);}

    if (x0 == x1) {
        image.SetPixel(x0, y, z0, color);
        return;
    }

    float dz = (z1 - z0) / (x1 - x0);
    float z = z0;
    for (int x = x0; x <= x1; x++) {
        image.SetPixel(x, y, z, color);
        z += dz;
    }
}

void Rasterizer::DrawVerticalLine(FrameBuffer& image, int y0, int y1, float z0, float z1, int x, uint32_t color) {
    if (y0 > y1) {std::swap(y0, y1); std::swap(z0, z1);}

    if (y0 == y1) {
        image.SetPixel(x, y0, z0, color);
        return;
    }

    float dz = (z1 - z0) / (y1 - y0);
    float z = z0;
    for (int y = y0; y <= y1; y++) {
        image.SetPixel(x, y, z, color);
        z += dz;
    }
}

void Rasterizer::DrawTriangle2D(FrameBuffer& image, int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color)
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
        DrawHorizontalLine(image, (int)x_left, (int)x_right, y, color);
        x_left += dx_left1;
        x_right += dx_left2;
    }
    
    x_left = x2;
    
    for (int y = y2; y <= y3; y++) {
        DrawHorizontalLine(image, (int)x_left, (int)x_right, y, color);
        x_left += dx_right1;
        x_right += dx_left2;
    }
}

void Rasterizer::DrawTriangle3D(FrameBuffer& image, VertexScreen v0, VertexScreen v1, VertexScreen v2)
{
    auto iMin = [](int a,int b){ return a<b?a:b; };
    auto iMax = [](int a,int b){ return a>b?a:b; };

    int minX = iMin(v0.x, iMin(v1.x, v2.x));
    int maxX = iMax(v0.x, iMax(v1.x, v2.x));
    int minY = iMin(v0.y, iMin(v1.y, v2.y));
    int maxY = iMax(v0.y, iMax(v1.y, v2.y));

    minX = iMax(minX, 0);
    minY = iMax(minY, 0);
    maxX = iMin(maxX, image.GetWidth()  - 1);
    maxY = iMin(maxY, image.GetHeight() - 1);

    if (minX > maxX || minY > maxY) return;

    auto Edge = [](const VertexScreen& a, const VertexScreen& b, int x, int y) -> float {
        return float(x - a.x) * float(b.y - a.y) - float(y - a.y) * float(b.x - a.x);
    };

    float area = Edge(v0, v1, v2.x, v2.y);
    if (std::fabs(area) < 1e-8f) return;

    bool ccw = area > 0.0f;

    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {

            float w0 = Edge(v1, v2, x, y);
            float w1 = Edge(v2, v0, x, y);
            float w2 = Edge(v0, v1, x, y);

            if (ccw) {
                if (w0 < 0 || w1 < 0 || w2 < 0) continue;
            } else {
                if (w0 > 0 || w1 > 0 || w2 > 0) continue;
            }

            float l0 = w0 / area;
            float l1 = w1 / area;
            float l2 = w2 / area;

            float invW = l0*v0.invW + l1*v1.invW + l2*v2.invW;
            if (invW <= 0.0f) continue;

            float w = 1.0f / invW;

            // depth (persp-correct)
            float z = (l0*(v0.z*v0.invW) + l1*(v1.z*v1.invW) + l2*(v2.z*v2.invW)) * w;

            // color (persp-correct)
            Vec4 colorV = v0.color * l0 + v1.color * l1 + v2.color * l2;
            Vec4 rgba = colorV * w; // divide by invW

            uint32_t color = PackColor(rgba);
            image.SetPixel(x, y, z, color);
        }
    }
}

void Rasterizer::DrawTriangle3DScanline(FrameBuffer& image, VertexScreen v1, VertexScreen v2, VertexScreen v3)
{
    uint32_t v1col = PackColor(v1.color);
    uint32_t v2col = PackColor(v2.color);
    uint32_t v3col = PackColor(v3.color);
    if (v1col != v2col || v2col != v3col)
        std::cerr << "Color gradient not implemented. Please choose same color for all 3 vertexes" << std::endl;

    uint32_t color = v1col;

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
        DrawHorizontalLine(image, (int)x_left, (int)x_right, z_left, z_right, y, color);
        x_left += dx_left1;
        x_right += dx_left2;
        z_left += dz_left1;
        z_right += dz_left2;
    }
    
    x_left = v2.x;
    z_left = v2.z;
    
    for (int y = v2.y; y <= v3.y; y++) {
        DrawHorizontalLine(image, (int)x_left, (int)x_right, z_left, z_right, y, color);
        x_left += dx_right1;
        x_right += dx_left2;
        z_left += dz_right1;
        z_right += dz_left2;
    }
}

void Rasterizer::DrawRect(FrameBuffer& image, int x, int y, int width, int height, uint32_t color) {
    DrawTriangle2D(image, x, y, x + width, y, x, y + height, color);
    DrawTriangle2D(image, x, y + height, x + width, y, x + width, y + height, color);
}