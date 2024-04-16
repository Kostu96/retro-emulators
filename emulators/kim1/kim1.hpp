#pragma once
#include "shared/source/devices/cpu6502/cpu6502.hpp"

#include <array>
#include <span>

class KIM1
{
public:
    static constexpr u16 TEXTMODE_WIDTH = 22;
    static constexpr u16 TEXTMODE_HEIGHT = 23;
    static constexpr u16 SCREEN_WIDTH = TEXTMODE_WIDTH * 8;
    static constexpr u16 SCREEN_HEIGHT = TEXTMODE_HEIGHT * 8;

    KIM1();

    void clock();

    std::span<const u32> getScreenPixels() const { return m_screenPixels; }
private:
    u8 memoryRead(u16 address) const;
    void memoryWrite(u16 address, u8 data);

    u8 m_RAM[0x400];
    u8 m_RAM_HIGH[0x80];
    u8 m_FIRMWARE[0x800];

    CPU6502 m_cpu{};

    // RRIOT 2
    u8 m_PA2 = 0;
    u8 m_DDRA2 = 0;
    u8 m_PB2 = 0;
    u8 m_DDRB2 = 0;

    std::array<u32, SCREEN_WIDTH * SCREEN_HEIGHT> m_screenPixels;
};
