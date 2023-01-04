#include "io.hpp"

#include "cpu6502.hpp"

#include <iostream>
#include <iomanip>

constexpr u16 SYSTEM_TICKS = 20000; // 1MHz / 20000 = 50Hz

u8 IO::read(u16 offset)
{
    switch (offset)
    {
    case 0x10: return 0x80 | m_keyboard.getRow();
    case 0x11: return 0;
    case 0x12: return m_keyboard.getKey();
    case 0x20: return PIA2PortA;
    case 0x22: return PIA2PortB;
    case 0x40:
        VIAInterruptFlags &= ~(0x18);
        return VIAPortB;
    }

    std::cerr << "Unhandled load8 at IO: " << std::hex << std::setw(2) << std::setfill('0') << offset << '\n';
    return 0xCD;
}

void IO::write(u16 offset, u8 data)
{
    switch (offset)
    {
    case 0x10: {
        if (PIA1ControlA.bits.PortControl)
        {
            PIA1PortA = data;
            m_keyboard.setRow(data & 0x0F);
        }
        else
            PIA1DataDirectionA = data;
    } return;
    case 0x11:
        PIA1ControlA.byte = data;
        return;
    case 0x12: {
        if (PIA1ControlB.bits.PortControl)
            PIA1PortB = data;
        else
            PIA1DataDirectionB = data;
    } return;
    case 0x13:
        PIA1ControlB.byte = data;
        return;
    case 0x20: {
        if (PIA2ControlA.bits.PortControl)
            PIA2PortA = data;
        else
            PIA2DataDirectionA = data;
    } return;
    case 0x21:
        PIA2ControlA.byte = data;
        return;
    case 0x22: {
        if (PIA2ControlB.bits.PortControl)
            PIA2PortB = data;
        else
            PIA2DataDirectionB = data;
    } return;
    case 0x23:
        PIA2ControlB.byte = data;
        return;
    case 0x40:
        VIAPortB = data & VIADataDirectionB;
        VIAInterruptFlags &= ~0x18;
        return;
    case 0x41:
        VIAPortA = data & VIADataDirectionA;
        VIAInterruptFlags &= ~0x03;
        return;
    case 0x42:
        VIADataDirectionB = data;
        return;
    case 0x43:
        VIADataDirectionA = data;
        return;
    case 0x45:
        VIATimer1 = VIATimer1Latch;
        VIAInterruptFlags &= ~0x40;
        timer1 = true;
        return;
    case 0x4C:
        VIAPeripheralControl = data;
        return;
    case 0x4D:
        VIAInterruptFlags &= ~data;
        return;
    case 0x4E: {
        if (data & 0x80)
            VIAInterruptEnable |= data & 0x7f;
        else
            VIAInterruptEnable &= ~(data & 0x7f);
    } return;
    }

    std::cerr << "Unhandled store8 at IO: " << std::hex << std::setw(2) << std::setfill('0') << offset
        << " with: " << std::hex << std::setw(2) << std::setfill('0') << static_cast<u16>(data) << '\n';
}

void IO::clock()
{
    if (m_cycles++ == SYSTEM_TICKS)
    {
        m_cycles = 0;
        VIAPortB |= 0x20;
        if (!m_cpu.getFlags().bits.I)
            ;
        m_cpu.setIRQ(true);
    }
    else
    {
        VIAPortB &= ~0x20;
        m_cpu.setIRQ(false);
    }

    if (timer1)
    {
        VIATimer1--;

        if (VIATimer1 == 0xFFFF)
        {
            VIATimer1 = 0;
            timer1 = false;
            VIAInterruptFlags |= 0x40;
            m_cpu.setIRQ((VIAInterruptEnable & 0x80) && (VIAInterruptEnable & 0x40));
        }
        else
            m_cpu.setIRQ(false);
    }
}
