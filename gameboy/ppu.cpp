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
	case 0x1: return m_LCDStatus.byte;
	case 0x2: return m_SCY;
	case 0x3: return m_SCX;
	case 0x4: return m_LY;
	case 0x5: return m_LYC;

	case 0x7: return m_BGpaletteData;
	case 0x8: return m_OBJpalette0Data;
	case 0x9: return m_OBJpalette1Data;
	case 0xA: return m_WY;
	case 0xB: return m_WX;
	}

	assert(false && "Unhandled");
	return 0;
}

void PPU::store8(u16 address, u8 data)
{
	switch (address)
	{
	case 0x0: m_LCDControl.byte = data; return;
	case 0x1:
		m_LCDStatus.byte &= 0x0F;
		m_LCDStatus.byte |= data & 0xF0;
		return;
	case 0x2: m_SCY = data; return;
	case 0x3: m_SCX = data; return;

	case 0x5: m_LYC = data; return;

	case 0x7: m_BGpaletteData = data; return;
	case 0x8: m_OBJpalette0Data = data; return;
	case 0x9: m_OBJpalette1Data = data; return;
	case 0xA: m_WY = data; return;
	case 0xB: m_WX = data; return;
	}

	assert(false && "Unhandled");
}
