#pragma once
#include "cpu6502.hpp"

#include <span>

class Apple2
{
public:
    static constexpr u16 SCREEN_WIDTH = 40 * 8;
    static constexpr u16 SCREEN_HEIGHT = 24 * 8;

    Apple2();

    void update();

    std::span<u32> getScreenPixels() { return { m_screenPixels, SCREEN_WIDTH * SCREEN_HEIGHT }; }

    Apple2(const Apple2&) = delete;
    Apple2& operator=(const Apple2&) = delete;
private:
    u8 memoryRead(u16 address);
    void memoryWrite(u16 address, u8 data);
    bool handleSwitches(u16 address);

    u8 m_upperROM[0x2000];
    u8 m_RAM[0x1000];
    u8 m_characterROM[0x400];
    CPU6502 m_cpu;
    u32 m_screenPixels[SCREEN_WIDTH * SCREEN_HEIGHT];

    bool m_textGraphicsSwitch = false;
    bool m_mixedModeSwitch = false;
    bool m_pageSwitch = false;
    bool m_graphicsModeSwitch = false;
};
