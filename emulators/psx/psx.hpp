#pragma once
#include "shared/source/emulator_base.hpp"
#include "shared/source/imgui/disassembly_view.hpp" // TODO: to much spagetti
#include "cpu.hpp"

namespace PSX {

	constexpr u16 SCREEN_WIDTH = 600;
	constexpr u16 SCREEN_HEIGHT = 400;
	constexpr size_t RAM_SIZE = 2 * 1024 * 1024;
	constexpr size_t BIOS_SIZE = 512 * 1024;

	class Emulator :
		public EmulatorBase
	{
	public:
		void reset();
		void clock();

		const CPU& getCPU() const { return m_CPU; }

		explicit Emulator(Disassembly& disasm); // TODO: to much spagetti
	private:
		u8 memoryRead8(u32 address) const;
		u32 memoryRead32(u32 address) const;
		void memoryWrite8(u32 address, u8 data);
		void memoryWrite16(u32 address, u16 data);
		void memoryWrite32(u32 address, u32 data);

		u8 m_RAM[RAM_SIZE];
		u8 m_BIOS[BIOS_SIZE];
		CPU m_CPU;

		Disassembly& m_disasm;
	};

} // namespace PSX
