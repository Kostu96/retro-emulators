#include "bit_fifo.hpp"

bool BitFIFO::push(u8 byteL, u8 byteH)
{
	if (m_size <= 8)
	{
		m_bufferL |= byteL << (8 - m_size);
		m_bufferH |= byteH << (8 - m_size);
		m_size += 8;
		return true;
	}
		
	return false;
}

u8 BitFIFO::pop()
{
	u8 bitL = m_bufferL >> 15;
	m_bufferL <<= 1;
	u8 bitH = m_bufferH >> 15;
	m_bufferH <<= 1;
	m_size--;
	return (bitH << 1) | bitL;
}

void BitFIFO::clear()
{
	m_bufferL = 0;
	m_bufferH = 0;
	m_size = 0;
}
