#include "cia.hpp"

#include <cassert>

u8 CIA::load8(u16 address) const
{
    address &= 0xF; // mirror CIA adress range

    switch (address)
    {
    case 0x0: return m_PRA;
    case 0xE: return m_CRA;
    }

    assert(false);
    return 0;
}

void CIA::store8(u16 address, u8 data)
{
    address &= 0xF; // mirror CIA adress range

    switch (address)
    {
    case 0x0: m_PRA = data; return;
    case 0x2: m_DDRA = data; return;
    case 0x3: m_DDRB = data; return;
    case 0x4: m_TAL = data; return;
    case 0x5: m_TAH = data; return;
    case 0xD: m_ICR = data; return;
    case 0xE: m_CRA = data; return;
    case 0xF: m_CRB = data; return;
    }

    assert(false);
}
