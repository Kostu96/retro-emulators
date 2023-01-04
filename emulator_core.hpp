#pragma once
#include "types.hpp"

#include <span>

struct CharVertex {
    u16 x, y;
    u32 color;
};

class EmulatorCore
{
public:
    struct WindowSettings
    {
        u16 width;
        u16 height;
        char title[252];
    };

    virtual ~EmulatorCore() = default;

    virtual void getWindowSettings(WindowSettings& settings) = 0;
    virtual std::span<u8> getMemory() = 0;
    virtual void render(CharVertex* verts) = 0;

    virtual void loadROM(const char* filename) = 0;
    virtual void reset() = 0;
    virtual void clock() = 0;
};
