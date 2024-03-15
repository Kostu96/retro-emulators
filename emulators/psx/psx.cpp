#include "psx.hpp"

#include "../../shared/emu_common/include/emu_common/file_io.hpp"
#include "../../shared/address_range.hpp"

#include <cassert>
#include <iostream>
#include <iomanip>

#define HEX(value, w) "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(w) << value

namespace PSX {

	static constexpr AddressRange32 MEM_CTRL_RANGE{ 0x1F801000, 0x1F801023 };

	static constexpr AddressRange32 RAM_SIZE_RANGE{ 0x1F801060, 0x1F801063 };

	static constexpr AddressRange32 BIOS_RANGE{ 0xBFC00000, 0xBFC00000 + BIOS_SIZE - 1 };

	void Emulator::clock()
	{
		m_CPU.clock();
	}

	Emulator::Emulator()
	{
		size_t size = BIOS_SIZE;
		if (!readFile("SCPH-1001.bin", (char*)m_BIOS, size, true))
			std::cerr << "Could not read BIOS ROM file!\n";

		m_CPU.mapRead32MemoryCallback([this](u32 address) { return memoryRead32(address); });
		m_CPU.mapWrite32MemoryCallback([this](u32 address, u32 data) { memoryWrite32(address, data); });

		m_CPU.reset();
	}

	u32 Emulator::memoryRead32(u32 address) const
	{
		assert(address % 4 == 0 && "Unaligned memory access!");

		u32 offset;
		if (BIOS_RANGE.contains(address, offset)) {
			u32 b0 = m_BIOS[offset];
			u32 b1 = m_BIOS[offset + 1];
			u32 b2 = m_BIOS[offset + 2];
			u32 b3 = m_BIOS[offset + 3];
			return (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
		}

		std::cerr << "Unhandled read from memory at address: " << HEX(address, 8) << '\n';
		assert(false);
		return 0;
	}

	void Emulator::memoryWrite32(u32 address, u32 data)
	{
		assert(address % 4 == 0 && "Unaligned memory access!");

		u32 offset;
		if (RAM_SIZE_RANGE.contains(address, offset)) {
			std::cerr << "Unhandled 32 bit write to: " << HEX(address, 8) <<
				" RAM_SIZE(" << HEX(offset, 2) << "): " << HEX(data, 8) << '\n';
			return;
		}

		if (MEM_CTRL_RANGE.contains(address, offset)) {
			switch (offset) {
			case 0:
				if (data != 0x1F000000) {
					std::cerr << "Bad write to expansion 1 base address!\n";
					assert(false);
				}
				break;
			case 4:
				if (data != 0x1F802000) {
					std::cerr << "Bad write to expansion 2 base address!\n";
					assert(false);
				}
				break;
			default:
				std::cerr << "Unhandled 32 bit write to: " << HEX(address, 8) <<
					" MEM_CTRL(" << HEX(offset, 2) << "): " << HEX(data, 8) << '\n';
			}
			return;
		}

		std::cerr << "Unhandled write to memory at address: " << HEX(address, 8) << " data: " << HEX(data, 8) << '\n';
		assert(false);
	}

} // namespace PSX
