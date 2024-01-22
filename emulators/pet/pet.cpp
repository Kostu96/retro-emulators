#include "pet.hpp"
#include "address_range.hpp"

#include <ccl/helper_functions.h>
#include <cassert>
#include <iostream>

static constexpr AddressRange RAM_RANGE{           0x0000, PET::RAM_SIZE - 1 };
static constexpr AddressRange RAM_EXPANSION_RANGE{ PET::RAM_SIZE, 0x7FFF };
static constexpr AddressRange SCREEN_RANGE{        0x8000, 0x8FFF };

static constexpr AddressRange BASIC_RANGE{         0xE000 - PET::BASIC_SIZE, 0xDFFF };
static constexpr AddressRange EDITOR_RANGE{        0xE000, 0xE7FF };
static constexpr AddressRange PIA1_RANGE{          0xE810, 0xE81F };
static constexpr AddressRange PIA2_RANGE{          0xE820, 0xE82F };
static constexpr AddressRange VIA_RANGE{           0xE840, 0xE84F };
static constexpr AddressRange KERNAL_RANGE{        0xF000, 0xFFFF };

PET::PET()
{
#if BASIC_VER4
    static constexpr const char* basicPath = "builtin_roms/pet/basic4.bin";
    static constexpr const char* editorPath = "builtin_roms/pet/editor4n.bin";
    static constexpr const char* kernalPath = "builtin_roms/pet/kernal4.bin";
#else
    static constexpr const char* basicPath = "builtin_roms/pet/basic2.bin";
    static constexpr const char* editorPath = "builtin_roms/pet/editor2n.bin";
    static constexpr const char* kernalPath = "builtin_roms/pet/kernal2.bin";
#endif

    size_t size = PET::BASIC_SIZE;
    if (!ccl::readFile(basicPath, (char*)m_BASIC, size, true))
        std::cerr << "Could not read EDITOR ROM file!\n";

    size = 0x800;
    if (!ccl::readFile(editorPath, (char*)m_EDITOR, size, true))
        std::cerr << "Could not read EDITOR ROM file!\n";

    size = 0x1000;
    if (!ccl::readFile(kernalPath, (char*)m_KERNAL, size, true))
        std::cerr << "Could not read KERNAL ROM file!\n";
    
    size = 0x800;
    if (!ccl::readFile("builtin_roms/pet/characters2.bin", (char*)m_characters, size, true))
        std::cerr << "Could not read characters ROM file!\n";

    m_cpu.mapReadMemoryCallback([this](u16 address) { return memoryRead(address); });
    m_cpu.mapWriteMemoryCallback([this](u16 address, u8 data) { memoryWrite(address, data); });

    m_pia1.mapIRQBCallback([this](bool state) { m_cpu.setIRQ(state); });

    m_cpu.reset();
}

void PET::clock()
{
    constexpr u16 SYSTEM_TICKS = 16666; // 1MHz / 16666 = 60Hz

    static u16 counter = 0;

    counter++;

    m_cpu.clock();

    if (counter == SYSTEM_TICKS) {
        counter = 0;
        m_pia1.CB1();
    }
}

u8 PET::memoryRead(u16 address) const
{
    u16 offset;

    if (RAM_RANGE.contains(address, offset)) return m_RAM[offset];

    if (RAM_EXPANSION_RANGE.contains(address, offset)) return 0xFF;

    if (SCREEN_RANGE.contains(address, offset)) return m_SCREEN[offset];

    if (BASIC_RANGE.contains(address, offset)) return m_BASIC[offset];

    if (EDITOR_RANGE.contains(address, offset)) return m_EDITOR[offset];

    if (PIA1_RANGE.contains(address, offset)) return m_pia1.load8(offset);

    if (VIA_RANGE.contains(address, offset)) return m_via.load8(offset);

    if (KERNAL_RANGE.contains(address, offset)) return m_KERNAL[offset];

    assert(false);
    return 0;
}

void PET::memoryWrite(u16 address, u8 data)
{
    u16 offset;

    if (RAM_RANGE.contains(address, offset)) {
        m_RAM[offset] = data;
        return;
    }

    if (RAM_EXPANSION_RANGE.contains(address, offset)) return;

    if (SCREEN_RANGE.contains(address, offset)) {
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
                m_screenPixels[pixelOffset + (7 - j)] = (charData >> j) & 1 ? 0xFF50E050 : 0xFF000000;
            }
            pixelOffset += 8 * TEXTMODE_WIDTH;
        }

        return;
    }

    if (PIA1_RANGE.contains(address, offset)) {
        m_pia1.store8(offset, data);
        return;
    }

    if (PIA2_RANGE.contains(address, offset)) {
        m_pia2.store8(offset, data);
        return;
    }

    if (VIA_RANGE.contains(address, offset)) {
        m_via.store8(offset, data);
        return;
    }

    assert(false);
}
