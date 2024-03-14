#pragma once
#include "cpu.hpp"

namespace PSX {

	class Emulator :
		public ccl::NonCopyable
	{
	public:
		void clock();

		Emulator();
	private:
		static constexpr size_t BIOS_SIZE = 512 * 1024;

		u8 memoryRead(u32 address) const;
		void memoryWrite(u32 address, u8 data);

		u8 m_BIOS[BIOS_SIZE];
		CPU m_CPU;
	};

} // namespace PSX
