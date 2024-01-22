#include "cd_rom.h"

#include <iostream>

uint8_t CDROM::load8(uint32_t offset) const
{
	switch (offset) {
	case 0x0: return m_status;
	}

	__debugbreak();
	return 0xDE;
}

void CDROM::store8(uint32_t offset, uint8_t value)
{
	
	uint8_t index = m_status & 0x3;
	switch (offset) {
	case 0x0:
		m_status &= 0xFC;
		m_status |= value;
		return;
	case 0x2:
		switch (index) {
		case 0x1:
			m_interruptEnable = value;
			return;
		}
		break;
	case 0x3:
		switch (index) {
		case 0x1:
			m_interruptFlags = value;
			return;
		}
		break;
	}

	__debugbreak();
}
