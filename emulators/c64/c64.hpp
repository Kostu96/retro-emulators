#pragma once
#include "cpu6502/cpu6502.hpp"
#include "cia.hpp"
#include "sid.hpp"
#include "vic_ii.hpp"

namespace C64 {

    constexpr u16 SCREEN_WIDTH = 320;
    constexpr u16 SCREEN_HEIGHT = 200;

    class Emulator
    {
    public:
        Emulator();

        void clock();
    private:
        u8 memoryRead(u16 address) const;
        void memoryWrite(u16 address, u8 data);

        u8 m_KERNAL[0x2000];
        u8 m_characters[0x1000];
        u8 m_upperRAM[0x1000];
        u8 m_BASIC[0x2000];
        u8 m_RAM[0x10000];

        u8 m_cpuDDR = 0;
        u8 m_cpuPORT = 7; // Simulate pull-up resistors on system initialization
        CPU6502 m_cpu{};
        CIA m_cia1{};
        CIA m_cia2{};
        SID m_sid{};
        VICII m_vic{};
    };

} // namespace C64
