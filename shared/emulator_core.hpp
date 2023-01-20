#pragma once
#include "common.hpp"

#include <vector>

struct CharVertex {
    u16 x, y;
    u32 color;
};

struct StateEntry
{
    u16 value;
    u8 width;
    char label[13];
    bool sameLine = false;
    bool separator = false;
};

struct EmulatorSettings
{
    u16 frameWidth;
    u16 frameHeight;
    u16 windowWidth;
    u16 windowHeight;
    char windowTitle[248];
};

class EmulatorCore
{
public:
    virtual ~EmulatorCore() = default;

    virtual const EmulatorSettings& getEmulatorSettings() const = 0;

    virtual size_t getNumMemories() const { return 1; }
    virtual const size_t* getMemorySizes() const = 0;
    virtual const int* getMemoryColsNumbers() const = 0;
    virtual const std::vector<DisassemblyLine>& getDisassembly() const = 0;
    virtual const std::vector<StateEntry>& getState() const = 0;
    
    virtual u8 getByteAt(u16 address, size_t memoryIndex) const = 0;
    virtual u16 getPC() const = 0;

    virtual void render(CharVertex* verts) = 0;
    virtual void handleKey(int key, int action) = 0;

    virtual void loadROM(const char* filename) = 0;
    virtual void reset() = 0;
    virtual void update(double dt) = 0;
};
