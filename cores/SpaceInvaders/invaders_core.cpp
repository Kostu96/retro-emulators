#include "invaders_core.hpp"

#include <cassert>
#include <cstring>
#include <fstream>

static struct
{
#include "rom.inl"

    u8 read(u16 address) const { return rom[address]; }
} ROM;

static struct
{
    u8 ram[0x400];

    u8 read(u16 address) const { return ram[address]; }
    void write(u16 address, u8 data) { ram[address] = data; }
} RAM;

static struct
{
    u8 screen[0x1C00];

    u8 read(u16 address) const { return screen[address]; }
    void write(u16 address, u8 data) { screen[address] = data; }
} Screen;

extern "C"
{
    __declspec(dllexport) InvadersCore* allocator()
    {
        return new InvadersCore{};
    }

    __declspec(dllexport) void deleter(InvadersCore* ptr)
    {
        delete ptr;
    }
}

InvadersCore::InvadersCore() :
    m_cpu{ CPU8080::Mode::Intel8080 }
{
    m_cpu.map(ROM,    { 0x0000, 0x1FFF });
    m_cpu.map(RAM,    { 0x2000, 0x23FF });
    m_cpu.map(Screen, { 0x2400, 0x3FFF });
}

void InvadersCore::getWindowSettings(WindowSettings& settings)
{
    constexpr u16 CHIP8_WIDTH = 64;
    constexpr u16 CHIP8_HEIGHT = 32;
    constexpr u16 SCREEN_SIZE = CHIP8_WIDTH * CHIP8_HEIGHT;
    constexpr u16 ZOOM = 16;
    constexpr u16 WINDOW_WIDTH = CHIP8_WIDTH * ZOOM;
    constexpr u16 WINDOW_HEIGHT = CHIP8_HEIGHT * ZOOM;

    settings.width = WINDOW_WIDTH;
    settings.height = WINDOW_HEIGHT;
    strcpy_s(settings.title, "Space Invaders");
}

u8 InvadersCore::readByte(u16 address)
{
    return m_cpu.load8(address);
}

void InvadersCore::render(CharVertex* verts)
{
    
}

void InvadersCore::handleKey(int key, int action)
{
    constexpr int KEY1 = 49;
    constexpr int KEY2 = 50;
    constexpr int KEY3 = 51;
    constexpr int KEY4 = 52;
    constexpr int KEYQ = 81;
    constexpr int KEYW = 87;
    constexpr int KEYE = 69;
    constexpr int KEYR = 82;
    constexpr int KEYA = 65;
    constexpr int KEYS = 83;
    constexpr int KEYD = 68;
    constexpr int KEYF = 70;
    constexpr int KEYZ = 90;
    constexpr int KEYX = 88;
    constexpr int KEYC = 67;
    constexpr int KEYV = 86;
}

void InvadersCore::reset()
{
    m_cpu.reset();
}

void InvadersCore::clock()
{
    m_cpu.clock();
}
