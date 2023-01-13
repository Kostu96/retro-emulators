#pragma once
#include "type_aliases.hpp"

#include <vector>

struct CharVertex {
    u16 x, y;
    u32 color;
};

struct StateEntry
{
    u16 value;
    char label[14];
};

struct WindowSettings
{
    u16 width;
    u16 height;
    char title[252];
};

class EmulatorCore
{
public:
    virtual ~EmulatorCore() = default;

    virtual const WindowSettings& getWindowSettings() const = 0;

    virtual const std::vector<DisassemblyLine>& getDisassembly() const { return std::vector<DisassemblyLine>{}; } // TODO: temp - make abstract
    virtual const std::vector<StateEntry>& getState() const { return std::vector<StateEntry>{}; } // TODO: temp - make abstract
    
    virtual u8 getByteAt(u16 /*address*/) const { return 0xCD; };
    virtual u16 getPC() const { return 0; }

    virtual void render(CharVertex* verts) = 0;
    virtual void handleKey(int key, int action) = 0;

    virtual void loadROM(const char* filename) = 0;
    virtual void reset() = 0;
    virtual void clock() = 0;
};
