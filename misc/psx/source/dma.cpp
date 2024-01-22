#include "dma.h"

#include <iostream>

uint32_t DMA::Channel::getTransferSize() const
{
	uint32_t size = 0xDEADBEEF;
	if (control.fields.sync != Sync::LinkedList)
		size = blockControl.fields.blockSize;
	if (control.fields.sync == Sync::Request)
		size *= blockControl.fields.blockCount;

	return size;
}

void DMA::Channel::done()
{
	control.fields.enable = 0;
	control.fields.trigger = 0;
}

uint32_t DMA::load32(uint32_t offset) const
{
	switch (offset) {
	case 0x70: return m_control.word;
	case 0x74: return m_interrupt.word;
	}

	uint32_t upper = (offset & 0x70) >> 4;
	uint32_t lower = offset & 0xF;

	switch (lower) {
	case 0: return m_channels[upper].baseAddress.word;
	case 4: return m_channels[upper].blockControl.word;
	case 8: return m_channels[upper].control.word;
	}

	std::cerr << "Unhandled load from DMA: " << offset << '\n';
	return 0xDEADBEEF;
}

void DMA::store32(uint32_t offset, uint32_t value, DMA::Port& activePort)
{
	switch (offset) {
	case 0x70:
		m_control.word = value;
		return;
	case 0x74:
		m_interrupt.word = value;
		return;
	}

	uint32_t upper = (offset & 0x70) >> 4;
	uint32_t lower = offset & 0xF;

	switch (lower) {
	case 0:
		m_channels[upper].baseAddress.word = value;
		return;
	case 4:
		m_channels[upper].blockControl.word = value;
		return;
	case 8:
		m_channels[upper].control.word = value;
		bool trigger = (m_channels[upper].control.fields.sync == Channel::Sync::Manual) ?
			m_channels[upper].control.fields.trigger : true;
		if (trigger && m_channels[upper].control.fields.enable)
			activePort = static_cast<Port>(upper);
		return;
	}

	std::cerr << "Unhandled write to DMA: " << offset << '\n';
}
