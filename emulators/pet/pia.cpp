#include "pia.hpp"

#include <cassert>

u8 PIA::load8(u16 address) const
{
    switch (address)
    {
    case 0x0: return m_PRA;
    case 0x2: return m_PRB;
    }

    assert(false);
    return 0;
}

void PIA::store8(u16 address, u8 data)
{
    switch (address)
    {
    case 0x0: m_PRA = data; return;
    case 0x1: m_CA = data; return;
    case 0x2: m_PRB = data; return;
    case 0x3: m_CB = data; return;
    }

    assert(false);
}
