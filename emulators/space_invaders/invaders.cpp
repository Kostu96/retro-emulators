#include "invaders.hpp"
#include "../../shared/address_range.hpp"

#include <ccl/helper_functions.h>

#include <cassert>
#include <cstdio>
#include <cstring>
#include <fstream>

//#define CPU_DIAG

static const AddressRange ROM_RANGE{ 0x0000, 0x1FFF };
static const AddressRange RAM_RANGE{ 0x2000, 0x4000 };

//void Invaders::handleKey(int key, int action)
//{
//    constexpr int KEY1 = 49;
//    constexpr int KEY2 = 50;
//    constexpr int KEY3 = 51;
//    constexpr int KEY4 = 52;
//    constexpr int KEYQ = 81;
//    constexpr int KEYW = 87;
//    constexpr int KEYE = 69;
//    constexpr int KEYR = 82;
//    constexpr int KEYA = 65;
//    constexpr int KEYS = 83;
//    constexpr int KEYD = 68;
//    constexpr int KEYF = 70;
//    constexpr int KEYZ = 90;
//    constexpr int KEYX = 88;
//    constexpr int KEYC = 67;
//    constexpr int KEYV = 86;
//}

void Invaders::reset()
{
    m_cpu.reset();
}

void Invaders::update()
{
#ifdef CPU_DIAG
    if (m_cpu.getPC() == 0x0005)
    {
        u8 C = m_cpu.getBC() & 0xFF;
        u16 DE = m_cpu.getDE();

        if (C == 0x9)
        {
            char* str = (char*)&ROM.rom[DE];
            while (*str != '$')
                printf("%c", *str++);
        }
        else if (C == 0x2)
        {
            printf("%c", DE & 0xF);
        }
    }
#else
    if (m_cpu.getPC() == 0x1A5C)
    {
        std::memset(m_VRAM, 0, VRAM_SIZE);
    }
#endif

    m_cpu.clock();
}

Invaders::Invaders()
{
#ifdef CPU_DIAG
    std::ifstream file{ "C:\\Users\\kmisiak\\myplace\\retro-extras\\programs\\8080\\cpudiag.bin", std::ios_base::binary };
    assert(file.is_open());
    file.read((char*)(ROM.rom + 0x0100), 0x5A7);
    file.read((char*)RAM.ram, 0x1);
    file.close();

    ROM.rom[0x0000] = 0xC3;
    ROM.rom[0x0001] = 0x00;
    ROM.rom[0x0002] = 0x01;

    ROM.rom[0x0170] = 0x07;

    ROM.rom[0x0005] = 0xC9; // Early out from syscall

    m_cpu.map(ROM, { 0x0000, 0x06A5 });
    m_cpu.map(RAM, { 0x06A6, 0x16A5 });
#else
    std::ifstream romFile;
    constexpr size_t ROM_SIZE = 0x800;
    size_t offset = 0;
    for (auto filename : {
        "roms/invaders_h.bin",
        "roms/invaders_g.bin",
        "roms/invaders_f.bin",
        "roms/invaders_e.bin"
        })
    {
        size_t size = ROM_SIZE;
        [[maybe_unused]] bool ret = ccl::readFile(filename, (char*)(m_ROM + offset), size, true);
        assert(ret && "File read failed!");
        offset += ROM_SIZE;
    }

    m_ROM[0x1A5C] = 0xC9; // Early out from ClearScreen routine

    m_cpu.mapReadMemoryCallback([this](u16 address) { return memoryRead(address); });
    m_cpu.mapWriteMemoryCallback([this](u16 address, u8 data) { memoryWrite(address, data); });
#endif

    reset();
}

u8 Invaders::memoryRead(u16 address)
{
    u16 offset;

    if (ROM_RANGE.contains(address, offset)) return m_ROM[offset];
    if (RAM_RANGE.contains(address, offset)) return m_RAM[offset];

    assert(false && "Unhandled memory read!");
    return 0xCD;
}

void Invaders::memoryWrite(u16 address, u8 data)
{
    u16 offset;

    if (ROM_RANGE.contains(address, offset)) m_ROM[offset] = data; return;
    if (RAM_RANGE.contains(address, offset)) m_RAM[offset] = data; return;

    assert(false && "Unkhandled memory write");
}
