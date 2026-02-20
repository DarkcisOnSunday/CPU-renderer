#pragma once

#include <cstdint>

class IPresentSurface {
public:
    virtual ~IPresentSurface() = default;
    virtual void Present(const void* pixels, uint32_t w, uint32_t h) = 0;
    virtual void GetSize(uint32_t& w, uint32_t& h) const = 0;
};