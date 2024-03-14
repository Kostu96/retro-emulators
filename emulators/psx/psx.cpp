#include "psx.hpp"

#include <ccl/helper_functions.h>
#include <cassert>
#include <iostream>

namespace PSX {

	void Emulator::clock()
	{
		m_CPU.clock();
	}

	Emulator::Emulator()
	{
		size_t size = BIOS_SIZE;
		if (!ccl::readFile("SCPH-1001.bin", (char*)m_BIOS, size, true))
			std::cerr << "Could not read BIOS ROM file!\n";

		m_CPU.mapReadMemoryCallback([this](u32 address) { return memoryRead(address); });
		m_CPU.mapWriteMemoryCallback([this](u32 address, u8 data) { memoryWrite(address, data); });

		m_CPU.reset();
	}

	u8 Emulator::memoryRead(u32 address) const
	{
		return u8();
	}

	void Emulator::memoryWrite(u32 address, u8 data)
	{
	}

} // namespace PSX
