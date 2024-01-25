#pragma once
#include "cpu6502.hpp"
#include "pia.hpp"
#include "via.hpp"

#include <array>
#include <span>

#define BASIC_VER4 0
#define PETTEST 0

class PET
{
public:
    static constexpr u16 RAM_SIZE = 0x2000;
#if BASIC_VER4
    static constexpr u16 BASIC_SIZE = 0x3000;
#else
    static constexpr u16 BASIC_SIZE = 0x2000;
#endif
    static constexpr u16 TEXTMODE_WIDTH = 40;
    static constexpr u16 TEXTMODE_HEIGHT = 25;
    static constexpr u16 SCREEN_WIDTH = TEXTMODE_WIDTH * 8;
    static constexpr u16 SCREEN_HEIGHT = TEXTMODE_HEIGHT * 8;

    PET();

    void clock();

    std::span<const u32> getScreenPixels() const { return m_screenPixels; }
    void updateKeysFromEvent(int key, bool press);
    void updateKeysFromCodepoint(int codepoint);
private:
    u8 memoryRead(u16 address) const;
    void memoryWrite(u16 address, u8 data);

    u8 m_RAM[RAM_SIZE];
    u8 m_SCREEN[0x400];
    u8 m_BASIC[BASIC_SIZE];
    u8 m_EDITOR[0x800];
    u8 m_KERNAL[0x1000];
    u8 m_characters[0x800];

    CPU6502 m_cpu{};
    PIA m_pia1{};
    PIA m_pia2{};
    VIA m_via{};

    std::array<u32, SCREEN_WIDTH * SCREEN_HEIGHT> m_screenPixels;
    u8 m_keyRow = 0;
    u8 m_keyRows[10];
};
