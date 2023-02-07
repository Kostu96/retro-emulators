#include "timer.hpp"

#include <cassert>

void Timer::reset()
{
	m_divider = 0;
	m_counter = 0;
	m_modulo = 0;
}

void Timer::clock()
{
	m_divider++;

	static u32 counterAcc = 0;
	if (m_control.enable)
	{
		counterAcc += 4;

		u32 clock = 0;
		switch (m_control.clockSelect)
		{
		case 0: clock = 1024; break;
		case 1: clock = 16; break;
		case 2: clock = 64; break;
		case 3: clock = 256; break;
		}

		while (counterAcc >= clock)
		{
			m_counter++;
			if (m_counter == 0)
			{
				m_InterruptFlagsRef |= 4;
				m_counter = m_modulo;
			}
			counterAcc -= clock;
		}
	}
}

u8 Timer::load8(u16 address) const
{
	switch (address)
	{
	case 0: return m_divider >> 8;
	}

	assert(false);
	return u8();
}

void Timer::store8(u16 address, u8 data)
{
	switch (address)
	{
	case 0: m_divider = 0; return;
	case 1: m_counter = data; return;
	case 2: m_modulo = data; return;
	case 3: m_control.byte = data & 0x7; return;
	}

	assert(false);
}
