#include "ppu2C02.hpp"

#include <cassert>

void PPU2C02::reset()
{
    m_cycles = 0;
    m_scanlines = 0;
}

void PPU2C02::clock()
{
    m_cycles++;
    if (m_cycles == 341)
    {
        m_cycles = 0;
        m_scanlines++;
        if (m_scanlines == 261)
        {
            m_scanlines = -1;
        }
    }
}

u8 PPU2C02::read(u16 offset)
{
    u8 data = 0;

    switch (offset & 0x0007)
    {
    case 0: break;
    case 2:
        data = m_status.byte & 0xE0;
        m_status.fields.vertivalBlank = 0;
        break;
    default:
        assert(false);
    }

    return data;
}

void PPU2C02::write(u16 offset, u8 /*data*/)
{
    switch (offset & 0x0007)
    {
    case 0: break;
    default:
        assert(false);
    }
}
