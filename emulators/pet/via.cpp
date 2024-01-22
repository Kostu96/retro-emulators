#include "via.hpp"

#include <cassert>

u8 VIA::load8(u16 address) const
{
    switch (address)
    {
    case 0x0: return m_PRB;
    }

    assert(false);
    return 0;
}

void VIA::store8(u16 address, u8 data)
{
    switch (address)
    {
    case 0x0: m_PRB = data & m_DDRB; return;

    case 0x2: m_DDRB = data; return;

    case 0x5: m_timer1H = data; return;

    case 0xC: m_PCR = data; return;

    case 0xE: {
        if (data & 0x80)
            m_IER |= data & 0x7f;
        else
            m_IER &= ~(data & 0x7f);
    } return;
    }
    assert(false);
}
