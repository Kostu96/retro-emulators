#include "timer.hpp"

#include <cassert>

void Timer::reset()
{
	constexpr u16 DIVIDER_AFTER_BOOT = 0xABD0;
	constexpr u8 CONTROL_AFTER_BOOT = 0xF8;

	m_prevTriggerBit = 0;
	m_divider = DIVIDER_AFTER_BOOT;
	m_counter = 0;
	m_modulo = 0;
	m_control.byte = CONTROL_AFTER_BOOT;

	m_overflow = false;
	m_wasCounterWritten = false;
}

void Timer::clock()
{
	m_divider += 4;
	if (m_overflow) {
		m_overflow = false;
		if (!m_wasCounterWritten) {
			m_interruptFlagsRef |= 4;
			m_counter = m_modulo;
		}
	}

	if (m_wasCounterWritten) m_wasCounterWritten = false;

	u16 bit = 1;
	switch (m_control.clockSelect)
	{
	case 0: bit = 9; break;
	case 1: bit = 3; break;
	case 2: bit = 5; break;
	case 3: bit = 7; break;
	}

	u8 triggerBit = ((m_divider >> bit) & 1) & m_control.enable;
	bool shouldIncrement = m_prevTriggerBit & ~triggerBit;
	if (shouldIncrement)
	{
		m_counter++;
		if (m_counter == 0)
		{
			m_overflow = true;
		}
	}

	m_prevTriggerBit = triggerBit;
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
	case 1: m_counter = data; m_wasCounterWritten = true; return;
	case 2: m_modulo = data; return;
	case 3:
		m_control.byte &= 0xF8;
		m_control.byte |= data & 0x07;
		return;
	}

	assert(false);
}
