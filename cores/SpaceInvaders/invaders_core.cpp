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

    disassemble(ROM.rom, 0x2000, m_disassembly);

    m_state.push_back({ 0, "AF" });
    m_state.push_back({ 0, "BC" });
    m_state.push_back({ 0, "DE" });
    m_state.push_back({ 0, "HL" });
    m_state.push_back({ 0, "SP" });
    m_state.push_back({ 0, "PC" });
}

void InvadersCore::getWindowSettings(WindowSettings& settings) const
{
    constexpr u16 CHIP8_WIDTH = 256;
    constexpr u16 CHIP8_HEIGHT = 224;
    constexpr u16 SCREEN_SIZE = CHIP8_WIDTH * CHIP8_HEIGHT;
    constexpr u16 ZOOM = 2;
    constexpr u16 WINDOW_WIDTH = CHIP8_WIDTH * ZOOM;
    constexpr u16 WINDOW_HEIGHT = CHIP8_HEIGHT * ZOOM;

    settings.width = WINDOW_WIDTH;
    settings.height = WINDOW_HEIGHT;
    strcpy_s(settings.title, "Space Invaders");
}

u8 InvadersCore::getByteAt(u16 address) const
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

    m_state[0].value = m_cpu.getAF();
    m_state[1].value = m_cpu.getBC();
    m_state[2].value = m_cpu.getDE();
    m_state[3].value = m_cpu.getHL();
    m_state[4].value = m_cpu.getSP();
    m_state[5].value = m_cpu.getPC();
}
