#include "timer.hpp"

#include <cassert>

void Timer::reset()
{
	constexpr u16 DIVIDER_AFTER_BOOT = 0xABD4;
	constexpr u8 CONTROL_AFTER_BOOT = 0xF8;

	m_divider = DIVIDER_AFTER_BOOT;
	m_counter = 0;
	m_modulo = 0;
	m_control.byte = CONTROL_AFTER_BOOT;
}

void Timer::clock()
{
	u16 prevDivider = m_divider;
	m_divider += 4;

	if (m_control.enable)
	{
		u16 bit = 1;
		switch (m_control.clockSelect)
		{
		case 0: bit <<= 9; break;
		case 1: bit <<= 3; break;
		case 2: bit <<= 5; break;
		case 3: bit <<= 7; break;
		}

		bool shouldIncrement = (prevDivider & bit) & ~(m_divider & bit);
		if (shouldIncrement)
		{
			m_counter++;
			if (m_counter == 0)
			{
				m_interruptFlagsRef |= 4;
				m_counter = m_modulo;
			}
		}
	}
}

u8 Timer::load8(u16 address) const
{
	switch (address)
	{
	case 0: return m_divider >> 8;
	case 1: return m_counter;
	case 2: return m_modulo;
	case 3: return m_control.byte;
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
