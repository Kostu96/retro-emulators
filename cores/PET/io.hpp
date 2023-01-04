#pragma once
#include "../type_aliases.hpp"
#include "keyboard.hpp"

class CPU6502;

class IO
{
public:
    IO(CPU6502& cpu) :
        m_cpu{ cpu } {}

    u8 read(u16 offset);
    void write(u16 offset, u8 data);

    void clock();

    Keyboard& getKeyboard() { return m_keyboard; }
private:
    union Control
    {
        struct Bits
        {
            u8 Cx1ActiveTransition : 1;
            u8 Cx2ActiveTransition : 1;
            u8 Cx2Direction : 1;
            u8 Cx2Control : 2;
            u8 PortControl : 1;
            u8 Cx1Control : 2;
        };

        Bits bits;
        u8 byte = 0;
    };

    u8 PIA1PortA = 0;
    u8 PIA1DataDirectionA;
    Control PIA1ControlA{};
    u8 PIA1PortB = 0;
    u8 PIA1DataDirectionB;
    Control PIA1ControlB{};

    u8 PIA2PortA = 0;
    u8 PIA2DataDirectionA;
    Control PIA2ControlA{};
    u8 PIA2PortB = 0;
    u8 PIA2DataDirectionB;
    Control PIA2ControlB{};

    u8 VIAPortA;
    u8 VIADataDirectionA;
    u8 VIAPortB;
    u8 VIADataDirectionB;
    u16 VIATimer1 = 0;
    u16 VIATimer1Latch = 0;
    u16 VIATimer2 = 0;
    u8 VIAAUXControl;
    u8 VIAPeripheralControl;
    u8 VIAInterruptFlags = 0;
    u8 VIAInterruptEnable = 0x80;

    u16 m_cycles = 0;
    bool timer1 = false;
    bool timer2 = false;

    Keyboard m_keyboard;
    CPU6502& m_cpu;
};
