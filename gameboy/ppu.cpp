#include "ppu.hpp"

#include <cassert>

void PPU::reset()
{
	m_LY = 0x90; // TODO: temp
}

void PPU::clock()
{

}

u8 PPU::load8(u16 address)
{
	switch (address)
	{
	case 0x0: return m_LCDControl.byte;
	case 0x2: return m_SCY;
	case 0x3: return m_SCX;
	case 0x4: return m_LY;
	case 0x7: return m_paletteData;
	}

	assert(false && "Unhandled");
	return 0;
}

void PPU::store8(u16 address, u8 data)
{
	switch (address)
	{
	case 0x0: m_LCDControl.byte = data; return;
	case 0x2: m_SCY = data; return;
	case 0x3: m_SCX = data; return;
	case 0x7: m_paletteData = data; return;
	}

	assert(false && "Unhandled");
}
