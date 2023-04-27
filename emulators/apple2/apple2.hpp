#pragma once
#include "cpu6502.hpp"

class Apple2 :
    ccl::NonCopyable
{
public:
    Apple2();

    void update();
private:
    u8 memoryRead(u16 address);
    void memoryWrite(u16 address, u8 data);
    bool handleSwitches(u16 address);

    u8 m_upperROM[0x2000];
    u8 m_RAM[0x1000];
    u8 m_characterROM[0x800];
    CPU6502 m_cpu;

    bool m_textGraphicsSwitch = false;
    bool m_mixedModeSwitch = false;
    bool m_pageSwitch = false;
    bool m_graphicsModeSwitch = false;
};
