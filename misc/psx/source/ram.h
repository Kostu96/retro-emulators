#pragma once
#include <cstdint>

class RAM
{
public:
	RAM();
	~RAM();

	uint8_t load8(uint32_t offset) const;
	uint16_t load16(uint32_t offset) const;
	uint32_t load32(uint32_t offset) const;
	void store8(uint32_t offset, uint8_t value);
	void store16(uint32_t offset, uint16_t value);
	void store32(uint32_t offset, uint32_t value);
private:
	static const size_t SIZE = 2 * 1024 * 1024;
	uint8_t* m_data;
};
