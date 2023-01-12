#pragma once
#include "shared/type_aliases.hpp"

#include <span>
#include <vector>

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

    virtual void getWindowSettings(WindowSettings& settings) const = 0;
    virtual u8 getByteAt(u16 /*address*/) const { return 0xCD; };
    virtual u16 getPC() const { return 0; }
    virtual std::span<const u8> getMemory() const { return std::span<const u8>{}; }
    virtual const std::vector<DisassemblyLine>& getDisassembly() const { return std::vector<DisassemblyLine>{}; } // TODO: temp - make abstract
    
    virtual void render(CharVertex* verts) = 0;
    virtual void handleKey(int key, int action) = 0;

    virtual void loadROM(const char* filename) = 0;
    virtual void reset() = 0;
    virtual void clock() = 0;
};
