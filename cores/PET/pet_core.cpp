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

void PETCore::render(CharVertex* verts)
{

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
}

void PETCore::loadROM(const char* filename)
{

}

void PETCore::reset()
{
    m_cpu.reset();
}

void PETCore::clock()
{
    m_cpu.clock();
}

PETCore::PETCore() :
    m_io{ m_cpu },
    m_windowSettings{ 100, 100, "PET" }
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
