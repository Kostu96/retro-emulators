#pragma once
#include "cpu6502/cpu6502.hpp"

#include <array>
#include <span>

class VIC20
{
public:
    static constexpr u16 TEXTMODE_WIDTH = 22;
    static constexpr u16 TEXTMODE_HEIGHT = 23;
    static constexpr u16 SCREEN_WIDTH = TEXTMODE_WIDTH * 8;
    static constexpr u16 SCREEN_HEIGHT = TEXTMODE_HEIGHT * 8;

    VIC20();

    void clock();

    std::span<const u32> getScreenPixels() const { return m_screenPixels; }
private:
    u8 memoryRead(u16 address) const;
    void memoryWrite(u16 address, u8 data);

    u8 m_LOW_RAM[0x400];
    u8 m_RAM[0x1000];

    u8 m_CHARACTERS[0x1000];

    u8 m_BASIC[0x2000];
    u8 m_KERNAL[0x2000];

    CPU6502 m_cpu{};

    std::array<u32, SCREEN_WIDTH * SCREEN_HEIGHT> m_screenPixels;
};
