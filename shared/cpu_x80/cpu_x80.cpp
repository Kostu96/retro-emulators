#include "cpu_x80.hpp"

void CPUx80Base::reset()
{
    m_state.AF = 0;
    m_state.BC = 0;
    m_state.DE = 0;
    m_state.HL = 0;
    m_state.SP = 0xF000;
    m_state.PC = 0;
    m_state.InterruptEnabled = false;
    m_state.IsHalted = false;

    m_prefixMode = false;
    m_conditionalTaken = false;
    m_EIRequested = false;
    m_cyclesLeft = 0;
    m_interruptRequested = false;
}

bool CPUx80Base::interrupt(u8 vector)
{
    m_state.IsHalted = false;

    bool ret = m_state.InterruptEnabled;
    if (m_state.InterruptEnabled)
    {
        m_state.InterruptEnabled = false;
        m_interruptRequested = true;
        m_interruptVector = vector;
    }

    return ret;
}

void CPUx80Base::add8(u8 value, u8 carry, u16& result, u8& result4bit)
{
    result = m_state.A + value + carry;
    result4bit = (m_state.A & 0xF) + (value & 0xF) + carry;
    m_state.A = result;
}

void CPUx80Base::add16(u16 value, u32& result, u16& result12bit)
{
    result = m_state.HL + value;
    result12bit = (m_state.HL & 0xFFF) + (value & 0xFFF);
    m_state.HL = result;
}

void CPUx80Base::subtract(u8 value, u8 carry, u16& result, u8& result4bit)
{
    result = (s16)m_state.A - (s16)value - carry;
    result4bit = (s8)(m_state.A & 0xF) - (s8)(value & 0xF) - carry;
    m_state.A = result;
}

void CPUx80Base::compare(u8 value, u16& result, u8& result4bit)
{
    result = (s16)m_state.A - (s16)value;
    result4bit = (s8)(m_state.A & 0xF) - (s8)(value & 0xF);
}
