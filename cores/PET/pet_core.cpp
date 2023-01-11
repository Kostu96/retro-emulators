#include "pet_core.hpp"

#include <cassert>
#include <cstring>
#include <fstream>

extern "C"
{
    __declspec(dllexport) PETCore* allocator()
    {
        return new PETCore{};
    }

    __declspec(dllexport) void deleter(PETCore* ptr)
    {
        delete ptr;
    }
}

#define ROM_SERIES1

static struct {
    u8 memory[0x2000];

    u8 read(u16 address) const { return memory[address]; }
    void write(u16 address, u8 data) { memory[address] = data; }
} RAM;

static struct {
    u8 memory[0x400];

    u8 read(u16 address) const { return memory[address]; }
    void write(u16 address, u8 data) { memory[address] = data; }

    u8& operator[](u16 offset) { return memory[offset]; }
} VRAM;

static struct {
#if defined(ROM_SERIES1)
#include "roms/basic1_C000.inl"
#endif
#if defined(ROM_SERIES2)
#include "roms/basic2_C000.inl"
#endif
#if defined(ROM_SERIES4)
#include "roms/basic4_B000.inl"
#endif

        u8 read(u16 address) const { return BASIC[address]; }
} BASIC;

static struct {
#if defined(ROM_SERIES1)
#include "roms/editor1_E000.inl"
#endif
#if defined(ROM_SERIES2)
#include "roms/editor2_E000.inl"
#endif
#if defined(ROM_SERIES4)
#include "roms/editor4_E000.inl"
#endif

        u8 read(u16 address) const { return EDITOR[address]; }
} EDITOR;

static struct {
#if defined(ROM_SERIES1)
#include "roms/kernal1_F000.inl"
#endif
#if defined(ROM_SERIES2)
#include "roms/kernal2_F000.inl"
#endif
#if defined(ROM_SERIES4)
#include "roms/kernal4_F000.inl"
#endif

        u8 read(u16 address) const { return KERNAL[address]; }
} KERNAL;

PETCore::PETCore() :
    m_io{ m_cpu }
{
    m_cpu.map(RAM, { 0x0000, 0x1FFF });
    m_cpu.map(VRAM, { 0x8000, 0x83FF });
#if defined(ROM_SERIES4)
    m_cpu.map(BASIC, { 0xB000, 0xDFFF });
#else
    m_cpu.map(BASIC, { 0xC000, 0xDFFF });
#endif
    m_cpu.map(EDITOR, { 0xE000, 0xE7FF });
    m_cpu.map(m_io, { 0xE800, 0xEFFF });
    m_cpu.map(KERNAL, { 0xF000, 0xFFFF });
}

void PETCore::getWindowSettings(WindowSettings& settings)
{
    constexpr u16 CHIP8_WIDTH = 64;
    constexpr u16 CHIP8_HEIGHT = 32;
    constexpr u16 SCREEN_SIZE = CHIP8_WIDTH * CHIP8_HEIGHT;
    constexpr u16 ZOOM = 16;
    constexpr u16 WINDOW_WIDTH = CHIP8_WIDTH * ZOOM;
    constexpr u16 WINDOW_HEIGHT = CHIP8_HEIGHT * ZOOM;

    settings.width = WINDOW_WIDTH;
    settings.height = WINDOW_HEIGHT;
    strcpy_s(settings.title, "pet");
}

std::span<u8> PETCore::getMemory()
{
    return std::span<u8>{};
}

void PETCore::render(CharVertex* verts)
{
    /*for (u16 row = 0; row < 32; row++)
        for (u16 col = 0; col < 8; col++)
        {
            u16 screenIndex = row * 8 + col;
            for (u16 bit = 0; bit < 8; bit++)
            {
                u16 vertIndex = row * 64 + col * 8 + bit;
                bool on = Screen[screenIndex] & (1 << (7 - bit));
                verts[vertIndex].color = on ? 0xFFFFFFFF : 0;
            }
        }*/
}

void PETCore::handleKey(int key, int action)
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

    /*switch (key)
    {
    case KEY1: keys[0x1] = action; break;
    case KEY2: keys[0x2] = action; break;
    case KEY3: keys[0x3] = action; break;
    case KEY4: keys[0xF] = action; break;
    case KEYQ: keys[0x4] = action; break;
    case KEYW: keys[0x5] = action; break;
    case KEYE: keys[0x6] = action; break;
    case KEYR: keys[0xE] = action; break;
    case KEYA: keys[0x7] = action; break;
    case KEYS: keys[0x8] = action; break;
    case KEYD: keys[0x9] = action; break;
    case KEYF: keys[0xD] = action; break;
    case KEYZ: keys[0xA] = action; break;
    case KEYX: keys[0x0] = action; break;
    case KEYC: keys[0xB] = action; break;
    case KEYV: keys[0xC] = action; break;
    }*/
}

void PETCore::loadROM(const char* filename)
{
    //constexpr u8 CHARSET_SIZE = 80;
    //const u8 charset[CHARSET_SIZE] = {
    //    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    //    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    //    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    //    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    //    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    //    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    //    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    //    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    //    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    //    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    //    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    //    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    //    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    //    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    //    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    //    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    //};

    //std::memcpy(Memory, charset, CHARSET_SIZE);

    /*std::ifstream fin{ filename, std::ios::binary };
    fin.read((char*)(Memory + 0x200), 0x4A0);
    fin.close();*/
}

void PETCore::reset()
{
    m_cpu.reset();
}

void PETCore::clock()
{
    m_cpu.clock();
}
