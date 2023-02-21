#include "altair.hpp"

Altair::Altair() :
	m_CPU{ CPUx80Mode::Intel8080 },
	m_RAM{ new u8[0x10000] }
{
	m_CPU.mapReadMemoryCallback([this](u16 address) { return memoryRead(address); });
	m_CPU.mapWriteMemoryCallback([this](u16 address, u8 data) { memoryWrite(address, data); });

	reset();
}

Altair::~Altair()
{
	delete[] m_RAM;
}

void Altair::reset()
{
	m_CPU.reset();
}

void Altair::update()
{
	m_CPU.clock();
}

u8 Altair::memoryRead(u16 address)
{
	return m_RAM[address];
}

void Altair::memoryWrite(u16 address, u8 data)
{
	m_RAM[address] = data;
}
