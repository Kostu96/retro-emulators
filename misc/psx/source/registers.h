#pragma once
#include "type_helpers.h"

#include <cstdint>

class Registers
{
public:
    Registers() = default;
    Registers(const Registers& other);
    Registers& operator=(const Registers& other);

    void reset();

    void set(RegisterIndex reg, uint32_t value)
    {
        m_regs[reg.index] = value;
        m_regs[0] = 0;
    }

    uint32_t get(RegisterIndex reg) const { return m_regs[reg.index]; }
private:
	static const size_t SIZE = 32;
	uint32_t m_regs[SIZE]{};
};
