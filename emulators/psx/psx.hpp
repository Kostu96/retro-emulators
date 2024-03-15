#pragma once
#include "cpu.hpp"

namespace PSX {

	constexpr u16 SCREEN_WIDTH = 600;
	constexpr u16 SCREEN_HEIGHT = 400;
	constexpr size_t BIOS_SIZE = 512 * 1024;

	class Emulator
	{
	public:
		void clock();

		Emulator();
	private:
		//u8 memoryRead8(u32 address) const;
		u32 memoryRead32(u32 address) const;
		//void memoryWrite8(u32 address, u8 data);
		void memoryWrite32(u32 address, u32 data);

		u8 m_BIOS[BIOS_SIZE];
		CPU m_CPU;
	};

} // namespace PSX
