#pragma once
#include "../shared/type_aliases.hpp"

#include <ccl/non_copyable.h>

class Timer :
	public ccl::NonCopyable
{
public:
	Timer(u8& interruptFlagsRef) : m_InterruptFlagsRef{ interruptFlagsRef } {}

	void reset();
	void clock();

	u8 load8(u16 address) const;
	void store8(u16 address, u8 data);
private:
	u16 m_divider;
	u8 m_counter;
	u8 m_modulo;

	union {
		struct {
			u8 clockSelect : 2; // 0-1
			u8 enable      : 1; // 2
			u8 unused      : 5; // 3-7
		};
		u8 byte;
	} m_control;

	u8& m_InterruptFlagsRef;
};
