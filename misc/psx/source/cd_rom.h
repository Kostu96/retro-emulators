#pragma once
#include <cpp-common/non_copyable.h>
#include <cstdint>

class CDROM :
	public NonCopyable
{
public:
	CDROM() = default;

	uint8_t load8(uint32_t offset) const;
	void store8(uint32_t offset, uint8_t value);
private:
	uint8_t m_status;
	uint8_t m_interruptEnable;
	uint8_t m_interruptFlags;
};
