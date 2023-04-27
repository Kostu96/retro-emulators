#include "apple2.hpp"
#include "address_range.hpp"

#include <ccl/helper_functions.h>
#include <cassert>
#include <iostream>

static constexpr u16 TEXT_GRAPHICS_SWITCH_ON_ADDRESS = 0xC050;
static constexpr u16 TEXT_GRAPHICS_SWITCH_OFF_ADDRESS = 0xC051;
static constexpr u16 MIXED_MODE_SWITCH_ON_ADDRESS = 0xC052;
static constexpr u16 MIXED_MODE_SWITCH_OFF_ADDRESS = 0xC053;
static constexpr u16 PAGE_SWITCH_ON_ADDRESS = 0xC054;
static constexpr u16 PAGE_SWITCH_OFF_ADDRESS = 0xC055;
static constexpr u16 GRAPHICS_MODE_SWITCH_ON_ADDRESS = 0xC056;
static constexpr u16 GRAPHICS_MODE_SWITCH_OFF_ADDRESS = 0xC057;
static constexpr AddressRange RAM_RANGE{ 0x0000, 0x1000 };
static constexpr AddressRange UPPER_ROM_RANGE{ 0xE000, 0xFFFF };

Apple2::Apple2()
{
    size_t size = 0x800;
    if (!ccl::readFile("builtin_roms/apple2/Apple II Character ROM - 341-0036.bin",
        (char*)m_characterROM, size, true))
        std::cerr << "Could not read character ROM file!\n";

    size = 0x800;
    if (!ccl::readFile("builtin_roms/apple2/Apple II ROM Pages E0-E7 - 341-0001 - Integer BASIC.bin",
        (char*)m_upperROM, size, true))
        std::cerr << "Could not read Integer BASIC ROM 1 file!\n";

    size = 0x800;
    if (!ccl::readFile("builtin_roms/apple2/Apple II ROM Pages E8-EF - 341-0002 - Integer BASIC.bin",
        (char*)(m_upperROM + 0x800), size, true))
        std::cerr << "Could not read Integer BASIC ROM 2 file!\n";

    size = 0x800;
    if (!ccl::readFile("builtin_roms/apple2/Apple II ROM Pages F0-F7 - 341-0003 - Integer BASIC.bin",
        (char*)(m_upperROM + 0x1000), size, true))
        std::cerr << "Could not read Integer BASIC ROM 3 file!\n";

    size = 0x800;
    if (!ccl::readFile("builtin_roms/apple2/Apple II ROM Pages F8-FF - 341-0004 - Original Monitor.bin",
        (char*)(m_upperROM + 0x1800), size, true))
        std::cerr << "Could not read Origianl Monitor ROM file!\n";

    m_cpu.mapReadMemoryCallback([this](u16 address) { return memoryRead(address); });
    m_cpu.mapWriteMemoryCallback([this](u16 address, u8 data) { return memoryWrite(address, data); });

    m_cpu.reset();
}

void Apple2::update()
{
    m_cpu.clock();
}

u8 Apple2::memoryRead(u16 address)
{
    if (handleSwitches(address))
        return 0;

    u16 offset;
    if (RAM_RANGE.contains(address, offset)) return m_RAM[offset];
    if (UPPER_ROM_RANGE.contains(address, offset)) return m_upperROM[offset];

    assert(false && "Unhandled memory read!");
    return 0;
}

void Apple2::memoryWrite(u16 address, u8 data)
{
    if (handleSwitches(address))
        return;

    u16 offset;
    if (RAM_RANGE.contains(address, offset)) { m_RAM[offset] = data; return; }

    assert(false && "Unhandled memory write!");
}

bool Apple2::handleSwitches(u16 address)
{
    switch (address)
    {
    case TEXT_GRAPHICS_SWITCH_ON_ADDRESS:  m_textGraphicsSwitch = true;  break;
    case TEXT_GRAPHICS_SWITCH_OFF_ADDRESS: m_textGraphicsSwitch = false; break;
    case MIXED_MODE_SWITCH_ON_ADDRESS:     m_mixedModeSwitch = true;     break;
    case MIXED_MODE_SWITCH_OFF_ADDRESS:    m_mixedModeSwitch = false;    break;
    case PAGE_SWITCH_ON_ADDRESS:           m_pageSwitch = true;          break;
    case PAGE_SWITCH_OFF_ADDRESS:          m_pageSwitch = false;         break;
    case GRAPHICS_MODE_SWITCH_ON_ADDRESS:  m_graphicsModeSwitch = true;  break;
    case GRAPHICS_MODE_SWITCH_OFF_ADDRESS: m_graphicsModeSwitch = false; break;
    default:
        return false;
    }

    return true;
}
