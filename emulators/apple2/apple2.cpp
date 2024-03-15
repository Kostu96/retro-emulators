#include "apple2.hpp"
#include "../../address_range.hpp"
#include "emu_common/file_io.hpp"

#include <cassert>
#include <iostream>

static constexpr AddressRange16 RAM_RANGE{ 0x0000, 0x1000 };
static constexpr AddressRange16 TEXT_PAGE1_SUBRANGE{ 0x0400, 0x07FF };
static constexpr AddressRange16 KEYBOARD_DATA_RANGE{ 0xC000, 0xC00F };
static constexpr u16 SPEAKER_TOGGLE_ADDRESS = 0xC030;
static constexpr u16 TEXT_GRAPHICS_SWITCH_ON_ADDRESS = 0xC050;
static constexpr u16 TEXT_GRAPHICS_SWITCH_OFF_ADDRESS = 0xC051;
static constexpr u16 MIXED_MODE_SWITCH_ON_ADDRESS = 0xC052;
static constexpr u16 MIXED_MODE_SWITCH_OFF_ADDRESS = 0xC053;
static constexpr u16 PAGE_SWITCH_ON_ADDRESS = 0xC054;
static constexpr u16 PAGE_SWITCH_OFF_ADDRESS = 0xC055;
static constexpr u16 GRAPHICS_MODE_SWITCH_ON_ADDRESS = 0xC056;
static constexpr u16 GRAPHICS_MODE_SWITCH_OFF_ADDRESS = 0xC057;
static constexpr AddressRange16 ANNUNCIATORS_RANGE{ 0xC058, 0xC05F };
static constexpr AddressRange16 UPPER_ROM_RANGE{ 0xE000, 0xFFFF };

Apple2::Apple2()
{
    size_t size = 0x400;
    if (!readFile("builtin_roms/apple2/AppleII_CharacterROM.bin",
        (char*)m_characterROM, size, true))
        std::cerr << "Could not read character ROM file!\n";

    size = 0x800;
    if (!readFile("builtin_roms/apple2/Apple II ROM Pages E0-E7 - 341-0001 - Integer BASIC.bin",
        (char*)m_upperROM, size, true))
        std::cerr << "Could not read Integer BASIC ROM 1 file!\n";

    size = 0x800;
    if (!readFile("builtin_roms/apple2/Apple II ROM Pages E8-EF - 341-0002 - Integer BASIC.bin",
        (char*)(m_upperROM + 0x800), size, true))
        std::cerr << "Could not read Integer BASIC ROM 2 file!\n";

    size = 0x800;
    if (!readFile("builtin_roms/apple2/Apple II ROM Pages F0-F7 - 341-0003 - Integer BASIC.bin",
        (char*)(m_upperROM + 0x1000), size, true))
        std::cerr << "Could not read Integer BASIC ROM 3 file!\n";

    size = 0x800;
    if (!readFile("builtin_roms/apple2/Apple II ROM Pages F8-FF - 341-0004 - Original Monitor.bin",
        (char*)(m_upperROM + 0x1800), size, true))
        std::cerr << "Could not read Origianl Monitor ROM file!\n";

    m_cpu.mapReadMemoryCallback([this](u16 address) { return memoryRead(address); });
    m_cpu.mapWriteMemoryCallback([this](u16 address, u8 data) { return memoryWrite(address, data); });

    m_cpu.reset();

    for (auto& pixel : m_screenPixels)
        pixel = 0xFF;
}

void Apple2::update()
{
    m_cpu.clock();
}

u8 Apple2::memoryRead(u16 address)
{
    u16 offset;
    if (RAM_RANGE.contains(address, offset)) return m_RAM[offset];

    if (KEYBOARD_DATA_RANGE.contains(address, offset))
        return 0; // TODO: keyboard

    // TODO: temp
    if (ANNUNCIATORS_RANGE.contains(address, offset)) return 0;

    if (handleSwitches(address))
        return 0;

    if (UPPER_ROM_RANGE.contains(address, offset)) return m_upperROM[offset];

    assert(false && "Unhandled memory read!");
    return 0;
}

//#define BYTE_TO_BINARY_PATTERN " %c%c%c%c%c%c%c"
//#define BYTE_TO_BINARY(byte)  \
//  ((byte) & 0x40 ? 'X' : ' '), \
//  ((byte) & 0x20 ? 'X' : ' '), \
//  ((byte) & 0x10 ? 'X' : ' '), \
//  ((byte) & 0x08 ? 'X' : ' '), \
//  ((byte) & 0x04 ? 'X' : ' '), \
//  ((byte) & 0x02 ? 'X' : ' '), \
//  ((byte) & 0x01 ? 'X' : ' ') 

void Apple2::memoryWrite(u16 address, u8 data)
{
    if (handleSwitches(address))
        return;

    u16 offset;
    if (RAM_RANGE.contains(address, offset))
    {
        m_RAM[offset] = data;

        if (TEXT_PAGE1_SUBRANGE.contains(address, offset) && offset < 960)
        {
            u16 charAddr = data;
            charAddr &= 0x3F;
            charAddr <<= 3;

            u16 y_char_coord = offset / (SCREEN_WIDTH / 8);
            u16 x_char_coord = offset % (SCREEN_WIDTH / 8);

            for (u16 line = 0; line < 8; line++)
            {
                u8 byte = m_characterROM[charAddr + line];
                for (s8 bit = 7; bit > 0; bit--)
                {
                    u16 y_pixel_coord = y_char_coord * 8 + line;
                    u16 x_pixel_coord = x_char_coord * 8 + (7 - bit);
                    m_screenPixels[y_pixel_coord * SCREEN_WIDTH + x_pixel_coord] = ((byte >> bit) & 1) ? 0xFFFFFFFF : 0xFF;
                }
            }

            std::cout << std::endl;
        }

        return;
    }

    assert(false && "Unhandled memory write!");
}

bool Apple2::handleSwitches(u16 address)
{
    switch (address)
    {
    case SPEAKER_TOGGLE_ADDRESS:
        /* TODO: speaker */ break;
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
