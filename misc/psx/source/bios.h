#pragma once
#include "type_helpers.h"

#include <cstdint>

class BIOS
{
public:
	BIOS();
	~BIOS();

	uint8_t load8(uint32_t offset) const;
	uint32_t load32(uint32_t offset) const;
private:
	static const size_t SIZE = 512 * 1024;
	static const char* FILENAME;
	uint8_t* m_data;
};
