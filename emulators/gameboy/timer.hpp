#pragma once
#include <ccl/non_copyable.h>
#include <ccl/types.hpp>

class Timer :
	public ccl::NonCopyable
{
public:
	explicit Timer(u8& interruptFlagsRef) : m_interruptFlagsRef{ interruptFlagsRef } {}

	void reset();
	void clock();

	u8 load8(u16 address) const;
	void store8(u16 address, u8 data);
private:
	u8 m_prevTriggerBit;
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

	u8& m_interruptFlagsRef;
	bool m_overflow;
	bool m_wasCounterWritten;
};
