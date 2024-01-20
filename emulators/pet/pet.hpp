#pragma once
#include "cpu6502.hpp"
#include "pia.hpp"
#include "via.hpp"

#include <array>
#include <span>

class PET
{
public:
    static constexpr u32 SCREEN_WIDTH = 40 * 8;
    static constexpr u32 SCREEN_HEIGHT = 25 * 8;

    PET();

    void clock();

    std::span<const u32> getScreenPixels() const { return m_screenPixels; }
private:
    u8 memoryRead(u16 address) const;
    void memoryWrite(u16 address, u8 data);

    u8 m_RAM[0x1000];
    u8 m_SCREEN[0x400];
    u8 m_EDITOR[0x800];
    u8 m_KERNAL[0x1000];

    CPU6502 m_cpu{};
    PIA m_pia1{};
    PIA m_pia2{};
    VIA m_via{};

    std::array<u32, SCREEN_WIDTH * SCREEN_HEIGHT> m_screenPixels;
};
