#include "registers.h"

Registers::Registers(const Registers& other)
{
    for (unsigned int i = 0; i < SIZE; ++i)
        m_regs[i] = other.m_regs[i];
}

Registers& Registers::operator=(const Registers& other)
{
    for (unsigned int i = 0; i < SIZE; ++i)
        m_regs[i] = other.m_regs[i];

    return *this;
}

void Registers::reset()
{
    for (unsigned int i = 0; i < SIZE; ++i)
        m_regs[i] = 0xDEADBEEF;
}
