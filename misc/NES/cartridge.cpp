#include "cartridge.hpp"

#include "mapper_000.hpp"

#include <cassert>
#include <fstream>

constexpr u32 PRG_CHUNK_SIZE = 16384;
constexpr u32 CHR_CHUNK_SIZE = 8192;

void Cartridge::load(const char* filename)
{
	struct Header
	{
		u8 name[4];
		u8 numPRGChunks;
		u8 numCHRChunks;
		u8 mapper1;
		u8 mapper2;
		u8 prg_ram_size;
		u8 tv_system1;
		u8 tv_system2;
		u8 unused[5];
	} header;

    std::ifstream fin{ filename, std::ios::binary };
	assert(fin.is_open());

	fin.read((char*)&header, sizeof(header));
	
	if (header.mapper1 & 0x04)
		fin.seekg(512);

	m_programROM.resize(header.numPRGChunks * PRG_CHUNK_SIZE);
	fin.read((char*)m_programROM.data(), m_programROM.size());

	m_characterROM.resize(header.numCHRChunks * CHR_CHUNK_SIZE);
	fin.read((char*)m_characterROM.data(), m_characterROM.size());

	fin.close();

	u8 mapperID = (header.mapper2 & 0xF0) | (header.mapper1 >> 4);
	switch (mapperID)
	{
	case 0: m_mapper = std::make_unique<Mapper000>(header.numPRGChunks, header.numCHRChunks); break;
	}
}

u8 Cartridge::read(u16 offset)
{
	u8 data = 0;

	data = m_programROM[m_mapper->map(offset)];

    return data;
}

void Cartridge::write(u16 /*offset*/, u8 /*data*/)
{
}
