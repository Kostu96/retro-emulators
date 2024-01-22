#pragma once
#include <cpp-common/non_copyable.h>
#include <cstdint>

class Timers :
	public NonCopyable
{
public:
	Timers() = default;

	uint32_t load32(uint32_t offset) const;
	void store16(uint32_t offset, uint16_t value);
	void store32(uint32_t offset, uint32_t value);
private:
	struct CounterMode {
		uint16_t syncEnable              : 1;  // 0
		uint16_t syncMode                : 2;  // 1-2
		uint16_t resetMode               : 1;  // 3
		uint16_t targetResetIRQEnable    : 1;  // 4
		uint16_t overflowResetIRQEnable  : 1;  // 5
		uint16_t IRQRepeatMode           : 1;  // 6
		uint16_t IRQToggleMode           : 1;  // 7
		uint16_t clockSource             : 2;  // 8-9
		uint16_t interrupt               : 1;  // 10
		uint16_t reachedTarget           : 1;  // 11
		uint16_t reachedOverflow         : 1;  // 12
		uint16_t alwaysZero              : 3;  // 13-15
		uint16_t notUsed                 : 16; // 16-31
	};

	union Counter  {
		struct {
			uint16_t value;    // 0x00
			uint16_t notUsed1;
			CounterMode mode;  // 0x04
			uint16_t target;   // 0x08
			uint16_t notUsed2;
		};
		uint32_t as32[3];
		uint16_t as16[6];
	};

	Counter m_counters[3];
};
