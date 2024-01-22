#include "pia.hpp"

#include <cassert>

u8 PIA::load8(u16 address) const
{
    switch (address)
    {
    case 0x0: return 0x80 | m_PRA | m_DDRA;
    case 0x2:
        m_CRB.C1ActiveTransitionFlag = 0;
        m_IRQB(false);
        return 0xFF;// m_PRB | m_DDRB;
    }

    assert(false);
    return 0;
}

void PIA::store8(u16 address, u8 data)
{
    switch (address)
    {
    case 0x0: {
        if (m_CRA.PortControl) {
            m_PRA &= ~m_DDRA;
            m_PRA |= data & m_DDRA;
        } else
            m_DDRA = data;
    } return;
    case 0x1: m_CRA.byte = data; return;
    case 0x2: {
        if (m_CRB.PortControl) {
            m_PRB &= ~m_DDRB;
            m_PRB |= data & m_DDRB;
        } else
            m_DDRB = data;
    } return;
    case 0x3: m_CRB.byte = data; return;
    }

    assert(false);
}

void PIA::CB1()
{
    m_CRB.C1ActiveTransitionFlag = 1;
    if (m_CRB.C1IRQEnable) {
        m_IRQB(true);
    }
}
