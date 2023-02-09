#include "cartridge.hpp"

#include <ccl/helper_functions.h>

#include <cassert>
#include <cstring>
#include <iomanip>
#include <iostream>

static const char* CartridgeTypeCodeToStr(u8 code) {
	switch (code) {
	case 0x00: return "ROM ONLY";
	case 0x01: return "MBC1";
	case 0x02: return "MBC1 + RAM";
	case 0x03: return "MBC1 + RAM + BATTERY";

	case 0x05: return "MBC2";
	case 0x06: return "MBC2 + BATTERY";

	case 0x08: return "ROM + RAM 1";
	case 0x09: return "ROM + RAM + BATTERY 1";

	case 0x0B: return "MMM01";
	case 0x0C: return "MMM01 + RAM";
	case 0x0D: return "MMM01 + RAM + BATTERY";

	case 0x0F: return "MBC3 + TIMER + BATTERY";
	case 0x10: return "MBC3 + TIMER + RAM + BATTERY 2";
	case 0x11: return "MBC3";
	case 0x12: return "MBC3 + RAM 2";
	case 0x13: return "MBC3 + RAM + BATTERY 2";

	case 0x19: return "MBC5";
	case 0x1A: return "MBC5 + RAM";
	case 0x1B: return "MBC5 + RAM + BATTERY";
	case 0x1C: return "MBC5 + RUMBLE";
	case 0x1D: return "MBC5 + RUMBLE + RAM";
	case 0x1E: return "MBC5 + RUMBLE + RAM + BATTERY";

	case 0x20: return "MBC6";

	case 0x22: return "MBC7 + SENSOR + RUMBLE + RAM + BATTERY";

	case 0xFC: return "POCKET CAMERA";
	case 0xFD: return "BANDAI TAMA5";
	case 0xFE: return "HuC3";
	case 0xFF: return "HuC1 + RAM + BATTERY";
	}

	assert(false && "Unhandled code");
	return "DEADBEEF";
}

static size_t ROMSizeCodeToKB(u8 code) {
	switch (code) {
	case 0x00: return 32;
	case 0x01: return 64;
	case 0x02: return 128;
	case 0x03: return 256;
	case 0x04: return 512;
	case 0x05: return 1024;
	case 0x06: return 1024 * 2;
	case 0x07: return 1024 * 4;
	case 0x08: return 1024 * 8;
	}

	assert(false && "Unhandled code");
	return 0xDEADBEEF;
}

static u8 ROMSizeCodeToBankMask(u8 code) {
	switch (code) {
	case 0x00: return 0b1;
	case 0x01: return 0b11;
	case 0x02: return 0b111;
	case 0x03: return 0b1111;
	case 0x04: return 0b11111;
	}

	assert(false && "Unhandled code");
	return 0xDE;
}

static size_t RAMSizeCodeToKB(u8 code) {
	switch (code) {
	case 0x00: return 0;
	case 0x02: return 8;
	case 0x03: return 32;
	case 0x04: return 128;
	case 0x05: return 64;
	}

	assert(false && "Unhandled code");
	return 0xDEADBEEF;
}

Cartridge::~Cartridge()
{
	delete[] m_data;
}

u8 Cartridge::load8(u16 address) const
{
	switch (m_header->cartridgeTypeCode)
	{
	case 0:
		if (address < 0x8000)
			return m_data[address];
		break;
	case 1:
	case 2:
	case 3:
		if (address < 0x4000)
			return m_data[address];

		if (address < 0x8000) {
			u8 bankNumber = MBC1RomBank ? MBC1RomBank : 1;
			bankNumber &= ROMSizeCodeToBankMask(m_header->ROMSizeCode);
			return m_data[bankNumber * 0x4000 + (address - 0x4000)];
		}
		break;
	}
	
	assert(false);
	return 0;
}

void Cartridge::store8(u16 address, u8 data)
{
	switch (m_header->cartridgeTypeCode)
	{
	case 1:
	case 2:
	case 3:
		if (address >= 0x2000 && address < 0x4000) {
			MBC1RomBank = (data & 0x1F) ? (data & 0x1F) : 1;
			return;
		}
	}
	
	std::cerr << "Unexpected write to cartridge - " << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << address;
	std::cerr << ':' << std::hex << std::setw(2) << (u16)data << '\n';
}

bool Cartridge::loadFromFile(const char* filename)
{
	if (!ccl::readFile(filename, nullptr, m_size, true)) {
		std::cerr << "Failed to read size of cartridge ROM file!\n";
		return false;
	}

	if (m_size < 0x8000) {
		std::cerr << "Size of cartridge ROM is less than 32KB!\n";
		return false;
	}

	delete[] m_data;
	m_data = new uint8_t[m_size];
	if (!ccl::readFile(filename, (char*)m_data, m_size, true)) {
		std::cerr << "Failed to read cartridge ROM file!\n";
		return false;
	}

	m_header = (Header*)(m_data + 0x100);

	u16 x = 0;
	for (u16 i = 0x0134; i <= 0x014C; i++)
		x = x - m_data[i] - 1;

	if (m_header->CGBFlag == 0xC0)
		std::cerr << "WARNING: CGB only flag is set!";

	if (m_header->headerChecksum != (x & 0xFF))
		std::cerr << "WARNING: Header checksum test doen't pass!\n";

	if (ROMSizeCodeToKB(m_header->ROMSizeCode) * 1024 != m_size)
		std::cerr << "WARNING: Header ROM size is different than file size read!\n";

	std::cout << "Loaded cartridge ROM file: " << filename << '\n'
		<< "Size loaded: " << m_size / 1024 << "KB\n"
		<< "Header:\n"
		<< "  Title:    " << m_header->title << '\n'
		<< "  Type:     " << CartridgeTypeCodeToStr(m_header->cartridgeTypeCode) << '\n'
		<< "  ROM size: " << ROMSizeCodeToKB(m_header->ROMSizeCode) << "KB\n"
		<< "  RAM size: " << RAMSizeCodeToKB(m_header->RAMSizeCode) << "KB\n";

	return true;
}
