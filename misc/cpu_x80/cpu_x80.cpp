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
