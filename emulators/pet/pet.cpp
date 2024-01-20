#include "pet.hpp"
#include "address_range.hpp"

#include <ccl/helper_functions.h>
#include <cassert>
#include <iostream>

static constexpr AddressRange RAM_RANGE{ 0x0000, 0x1000 };
static constexpr AddressRange SCREEN_RANGE{ 0x8000, 0x8FFF };
static constexpr AddressRange EDITOR_RANGE{ 0xE000, 0xE7FF };
static constexpr AddressRange PIA1_RANGE{ 0xE810, 0xE81F };
static constexpr AddressRange PIA2_RANGE{ 0xE820, 0xE82F };
static constexpr AddressRange VIA_RANGE{ 0xE840, 0xE84F };
static constexpr AddressRange KERNAL_RANGE{ 0xF000, 0xFFFF };

PET::PET()
{
    size_t size = 0x800;
    if (!ccl::readFile("builtin_roms/pet/editor2n.bin", (char*)m_EDITOR, size, true))
        std::cerr << "Could not read EDITOR ROM file!\n";

    size = 0x1000;
    if (!ccl::readFile("builtin_roms/pet/kernal2.bin", (char*)m_KERNAL, size, true))
        std::cerr << "Could not read KERNAL ROM file!\n";

    m_cpu.mapReadMemoryCallback([this](u16 address) { return memoryRead(address); });
    m_cpu.mapWriteMemoryCallback([this](u16 address, u8 data) { return memoryWrite(address, data); });

    m_cpu.reset();
}

void PET::clock()
{
    m_cpu.clock();
}

u8 PET::memoryRead(u16 address) const
{
    u16 offset;

    if (RAM_RANGE.contains(address, offset)) return m_RAM[offset];

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

    if (SCREEN_RANGE.contains(address, offset)) {
        offset &= 0x3FF;
        m_SCREEN[offset] = data;
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
