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

    CPU6502 m_cpu;
    u8 m_RAM[0x1000];

    bool m_textGraphicsSwitch = false;
    bool m_mixedModeSwitch = false;
    bool m_pageSwitch = false;
    bool m_graphicsModeSwitch = false;
};
