#include "kim1.hpp"
#include "shared/source/address_range.hpp"
#include "shared/source/file_io.hpp"

#include <cassert>
#include <iostream>

static constexpr AddressRange16 RAM_RANGE{      0x0000, 0x03FF };

static constexpr AddressRange16 RRIOT1_RANGE{   0x1700, 0x173F };
static constexpr AddressRange16 RRIOT2_RANGE{   0x1740, 0x177F };
static constexpr AddressRange16 RAM_HIGH_RANGE{ 0x1780, 0x17FF };

static constexpr AddressRange16 FIRMWARE_RANGE{ 0x1800, 0x1FFF };

KIM1::KIM1()
{
    size_t size = 0x400;
    if (!readFile("rom/kim1/firmware_1800-1BFF.bin", (char*)m_FIRMWARE, size, true))
        std::cerr << "Could not read firmware_1800-1BFF.bin ROM file!\n";

    size = 0x400;
    if (!readFile("rom/kim1/firmware_1C00-1FFF.bin", (char*)(m_FIRMWARE + 0x400), size, true))
        std::cerr << "Could not read firmware_1C00-1FFF.bin ROM file!\n";

    m_cpu.mapReadMemoryCallback([this](u16 address) { return memoryRead(address); });
    m_cpu.mapWriteMemoryCallback([this](u16 address, u8 data) { return memoryWrite(address, data); });

    m_cpu.reset();
}

void KIM1::clock()
{
    m_cpu.clock();
}

u8 KIM1::memoryRead(u16 address) const
{
    address &= 0x1FFF;

    u16 offset;

    if (RAM_RANGE.contains(address, offset)) return m_RAM[offset];

    if (RRIOT2_RANGE.contains(address, offset)) {
        switch (offset)
        {
        case 0x0: return m_PA2;
        default:
            assert(false);
        }
    }

    if (RAM_HIGH_RANGE.contains(address, offset)) return m_RAM_HIGH[offset];

    if (FIRMWARE_RANGE.contains(address, offset)) return m_FIRMWARE[offset];

    assert(false);
    return 0;
}

void KIM1::memoryWrite(u16 address, u8 data)
{
    address &= 0x1FFF;

    u16 offset;

    if (RAM_RANGE.contains(address, offset)) {
        m_RAM[offset] = data;
        return;
    }

    if (RRIOT1_RANGE.contains(address, offset)) {
        assert(false);
        return;
    }

    if (RRIOT2_RANGE.contains(address, offset)) {
        switch (offset)
        {
        case 0x0: m_PA2 = data; break;
        case 0x1: m_DDRA2 = data; break;
        case 0x2: m_PB2 = data; break;
        case 0x3: m_DDRB2 = data; break;
        default:
            assert(false);
        }
        return;
    }

    if (RAM_HIGH_RANGE.contains(address, offset)) {
        m_RAM_HIGH[offset] = data;
        return;
    }

    assert(false);
}
