#include "pia.hpp"

#include <cassert>

u8 PIA::load8(u16 address) const
{
    switch (address)
    {
    case 0x0: return m_PRA | m_DDRA;
    case 0x2: return m_PRB | m_DDRB;
    }

    assert(false);
    return 0;
}

void PIA::store8(u16 address, u8 data)
{
    switch (address)
    {
    case 0x0: {
        if (m_CRA.PortControl)
            m_PRA |= data & m_DDRA;
        else
            m_DDRA = data;
    } return;
    case 0x1: m_CRA.byte = data; return;
    case 0x2: {
        if (m_CRB.PortControl)
            m_PRB |= data & m_DDRB;
        else                  
            m_DDRB = data;
    } return;
    case 0x3: m_CRB.byte = data; return;
    }

    assert(false);
}
