#include "timers.h"

#include <iostream>

uint32_t Timers::load32(uint32_t offset) const
{
	size_t counterIndex = offset >> 4;
	size_t fieldIndex = (offset & 0xF) / 4;
	return m_counters[counterIndex].as32[fieldIndex];
}

void Timers::store16(uint32_t offset, uint16_t value)
{
	size_t counterIndex = offset >> 4;
	size_t fieldIndex = (offset & 0xF) / 2;
	m_counters[counterIndex].as16[fieldIndex] = value;
}

void Timers::store32(uint32_t offset, uint32_t value)
{
	size_t counterIndex = offset >> 4;
	size_t fieldIndex = (offset & 0xF) / 4;
	m_counters[counterIndex].as32[fieldIndex] = value;
}
