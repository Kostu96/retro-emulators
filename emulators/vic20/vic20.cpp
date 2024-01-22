#include "vic20.hpp"
#include "address_range.hpp"

#include <ccl/helper_functions.h>
#include <cassert>
#include <iostream>

static constexpr AddressRange LOW_RAM_RANGE{     0x0000, 0x03FF };
static constexpr AddressRange BLOCK0_OPEN_RANGE{ 0x0400, 0x0FFF };
static constexpr AddressRange RAM_RANGE{         0x1000, 0x1FFF };
static constexpr AddressRange BLOCKS1_3_RANGE{   0x2000, 0x7FFF };
static constexpr AddressRange CHARACTERS_RANGE{  0x8000, 0x8FFF };

static constexpr AddressRange BLOCK5_RANGE{      0xA000, 0xBFFF };
static constexpr AddressRange BASIC_RANGE{       0xC000, 0xDFFF };
static constexpr AddressRange KERNAL_RANGE{      0xE000, 0xFFFF };

VIC20::VIC20()
{
    size_t size = 0x1000;
    if (!ccl::readFile("builtin_roms/vic20/characters.bin", (char*)m_CHARACTERS, size, true))
        std::cerr << "Could not read characters ROM file!\n";

    size = 0x2000;
    if (!ccl::readFile("builtin_roms/vic20/basic2.bin", (char*)m_BASIC, size, true))
        std::cerr << "Could not read BASIC ROM file!\n";

    size = 0x2000;
    if (!ccl::readFile("builtin_roms/vic20/kernal_rev7.bin", (char*)m_KERNAL, size, true))
        std::cerr << "Could not read KERNAL ROM file!\n";

    m_cpu.mapReadMemoryCallback([this](u16 address) { return memoryRead(address); });
    m_cpu.mapWriteMemoryCallback([this](u16 address, u8 data) { return memoryWrite(address, data); });

    m_cpu.reset();
}

void VIC20::clock()
{
    m_cpu.clock();
}

u8 VIC20::memoryRead(u16 address) const
{
    u16 offset;

    if (LOW_RAM_RANGE.contains(address, offset)) return m_LOW_RAM[offset];

    if (BLOCK0_OPEN_RANGE.contains(address, offset)) return 0xFF;

    if (RAM_RANGE.contains(address, offset)) return m_LOW_RAM[offset];

    if (BLOCKS1_3_RANGE.contains(address, offset)) return 0xFF;

    if (CHARACTERS_RANGE.contains(address, offset)) return m_CHARACTERS[offset];

    if (BLOCK5_RANGE.contains(address, offset)) return 0xFF;

    //if (BASIC_RANGE.contains(address, offset)) return m_BASIC[offset];

    if (KERNAL_RANGE.contains(address, offset)) return m_KERNAL[offset];

    assert(false);
    return 0;
}

void VIC20::memoryWrite(u16 address, u8 data)
{
    u16 offset;

    if (LOW_RAM_RANGE.contains(address, offset)) {
        m_LOW_RAM[offset] = data;
        return;
    }

    if (BLOCK0_OPEN_RANGE.contains(address, offset)) return;

    if (RAM_RANGE.contains(address, offset)) {
        m_RAM[offset] = data;
        return;
    }

    if (BLOCKS1_3_RANGE.contains(address, offset)) return;

    if (CHARACTERS_RANGE.contains(address, offset)) return;

    /*if (SCREEN_RANGE.contains(address, offset)) {
        offset &= 0x3FF;
        m_SCREEN[offset] = data;

        if (offset > 999) return;

        u16 charDataOffset = data;
        charDataOffset <<= 3;

        u16 pixelX = (offset % TEXTMODE_WIDTH) * 8;
        u16 pixelY = (offset / TEXTMODE_WIDTH) * 8;
        u16 pixelOffset = pixelY * SCREEN_WIDTH + pixelX;
        for (u16 i = 0; i < 8; i++)
        {
            u8 charData = m_characters[charDataOffset++];
            for (s16 j = 7; j >= 0; j--)
            {
                m_screenPixels[pixelOffset + (7 - j)] = (charData >> j) & 1 ? 0xFFFFFF : 0xFF000000;
            }
            pixelOffset += 8 * TEXTMODE_WIDTH;
        }

        return;
    }*/

    assert(false);
}
