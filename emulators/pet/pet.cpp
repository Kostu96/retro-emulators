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
static constexpr AddressRange CRTC_RANGE{          0xE880, 0xE88F };
static constexpr AddressRange KERNAL_RANGE{        0xF000, 0xFFFF };

PET::PET()
{
#if BASIC_VER4
    static constexpr const char* basicPath = "builtin_roms/pet/basic4.bin";
#if PETTEST
    static constexpr const char* editorPath = "rom/tests/PETTESTE2KV04.bin";
#else
    static constexpr const char* editorPath = "builtin_roms/pet/editor4n.bin";
#endif
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
    m_pia1.mapPortAOutputCallback([this](u8 data) { m_keyRow = data; });
    m_pia1.mapPortBInputCallback([this]() -> u8 { return (m_keyRow < 10) ? m_keyRows[m_keyRow] : 0xFF; });

    for (size_t i = 0; i < 10; i++)
        m_keyRows[i] = 0xFF;

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

void PET::updateKeysFromEvent(int key, bool press, bool ctrl)
{
    if (!press) {
        for (size_t i = 0; i < 10; i++)
            m_keyRows[i] = 0xFF;
    }
    else {
        switch (key)
        {
        case 256: m_keyRows[9] = 0xEF; break; // escape
        case 257: m_keyRows[6] = 0xDF; break; // enter
        case 261: m_keyRows[1] = 0x7F; break; // delete
        case 262: m_keyRows[0] = 0x7F; break; // arrow right
        case 264: m_keyRows[1] = 0xBF; break; // arrow down
        case 268: m_keyRows[0] = 0xBF; break; // home
        case 340:
            if (ctrl)
                m_keyRows[8] = 0xFE;  // left shift
            break;
        case 344:
            if (ctrl)
                m_keyRows[8] = 0xDF; // right shift
            break;
        }
    }
}

void PET::updateKeysFromCodepoint(int codepoint)
{
    switch (codepoint)
    {
    case 32:  m_keyRows[9] = 0xFB; break; // 'space'
    case 33:  m_keyRows[0] = 0xFE; break; // '!'
    case 34:  m_keyRows[1] = 0xFE; break; // '"'
    case 35:  m_keyRows[0] = 0xFD; break; // '#'
    case 36:  m_keyRows[1] = 0xFD; break; // '$'
    case 37:  m_keyRows[0] = 0xFB; break; // '%'
    case 38:  m_keyRows[0] = 0xF7; break; // '&'
    case 39:  m_keyRows[1] = 0xFB; break; // '''
    case 40:  m_keyRows[0] = 0xEF; break; // '('
    case 41:  m_keyRows[1] = 0xEF; break; // ')'
    case 42:  m_keyRows[5] = 0x7F; break; // '*'
    case 43:  m_keyRows[7] = 0x7F; break; // '+'
    case 44:  m_keyRows[7] = 0xF7; break; // ','
    case 45:  m_keyRows[8] = 0x7F; break; // '-'
    case 46:  m_keyRows[9] = 0xBF; break; // '.'
    case 47:  m_keyRows[3] = 0x7F; break; // '/'
    case 48:  m_keyRows[8] = 0xBF; break; // '0'
    case 49:  m_keyRows[6] = 0xBF; break; // '1'
    case 50:  m_keyRows[7] = 0xBF; break; // '2'
    case 51:  m_keyRows[6] = 0x7F; break; // '3'
    case 52:  m_keyRows[4] = 0xBF; break; // '4'
    case 53:  m_keyRows[5] = 0xBF; break; // '5'
    case 54:  m_keyRows[4] = 0x7F; break; // '6'
    case 55:  m_keyRows[2] = 0xBF; break; // '7'
    case 56:  m_keyRows[3] = 0xBF; break; // '8'
    case 57:  m_keyRows[2] = 0x7F; break; // '9'
    case 58:  m_keyRows[5] = 0xEF; break; // ':'
    case 59:  m_keyRows[6] = 0xEF; break; // ';'
    case 60:  m_keyRows[9] = 0xF7; break; // '<'
    case 61:  m_keyRows[9] = 0x7F; break; // '='
    case 62:  m_keyRows[8] = 0xEF; break; // '>'
    case 63:  m_keyRows[7] = 0xEF; break; // '?'
    case 64:  m_keyRows[8] = 0xFD; break; // '@'

    case 91:  m_keyRows[9] = 0xFD; break; // '['
    case 92:  m_keyRows[1] = 0xF7; break; // '\'
    case 93:  m_keyRows[8] = 0xFB; break; // ']'
    case 94:  m_keyRows[2] = 0xDF; break; // '^'
    case 95:  m_keyRows[0] = 0xDF; break; // '_'

    case 97:  m_keyRows[4] = 0xFE; break; // 'a'
    case 98:  m_keyRows[6] = 0xFB; break; // 'b'
    case 99:  m_keyRows[6] = 0xFD; break; // 'c'
    case 100: m_keyRows[4] = 0xFD; break; // 'd'
    case 101: m_keyRows[2] = 0xFD; break; // 'e'
    case 102: m_keyRows[5] = 0xFD; break; // 'f'
    case 103: m_keyRows[4] = 0xFB; break; // 'g'
    case 104: m_keyRows[5] = 0xFB; break; // 'h'
    case 105: m_keyRows[3] = 0xF7; break; // 'i'
    case 106: m_keyRows[4] = 0xF7; break; // 'j'
    case 107: m_keyRows[5] = 0xF7; break; // 'k'
    case 108: m_keyRows[4] = 0xEF; break; // 'l'
    case 109: m_keyRows[6] = 0xF7; break; // 'm'
    case 110: m_keyRows[7] = 0xFB; break; // 'n'
    case 111: m_keyRows[2] = 0xEF; break; // 'o'
    case 112: m_keyRows[3] = 0xEF; break; // 'p'
    case 113: m_keyRows[2] = 0xFE; break; // 'q'
    case 114: m_keyRows[3] = 0xFD; break; // 'r'
    case 115: m_keyRows[5] = 0xFE; break; // 's'
    case 116: m_keyRows[2] = 0xFB; break; // 't'
    case 117: m_keyRows[2] = 0xF7; break; // 'u'
    case 118: m_keyRows[7] = 0xFD; break; // 'v'
    case 119: m_keyRows[3] = 0xFE; break; // 'w'
    case 120: m_keyRows[7] = 0xFE; break; // 'x'
    case 121: m_keyRows[3] = 0xFB; break; // 'y'
    case 122: m_keyRows[6] = 0xFE; break; // 'z'
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
                if (data & 0x80)
                    m_screenPixels[pixelOffset + (7 - j)] = (charData >> j) & 1 ? 0xFF000000 : 0xFF50E050;
                else
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

    if (CRTC_RANGE.contains(address, offset)) return;

    assert(false);
}
