#include "cpu4040.hpp"

#include <cassert>

void CPU4040::reset()
{

}

void CPU4040::clock()
{
    u8 opcode = load8(getPC());

    switch (opcode)
    {
    default:
        assert(false && "Unhandled instruction");
    }
}

CPU4040::CPU4040(Mode mode) :
    m_mode{ mode },
    REGS_SIZE{ mode == Mode::Intel4004 ? 16u : 24u },
    STACK_SIZE{ mode == Mode::Intel4004 ? 4u : 8u },
    m_regs{ new u8[REGS_SIZE] },
    m_stack{ new u16[STACK_SIZE] },
    SP{ 0 }
{

}

CPU4040::~CPU4040()
{
    delete[] m_regs;
    delete[] m_stack;
}

u8 CPU4040::load8(u16 address) const
{
    u8 data = 0;
    bool read = false;
    for (auto& entry : m_readMap)
    {
        u16 offset;
        if (entry.range.contains(address, offset))
        {
            data = entry.read(offset);
            read = true;
            break;
        }
    }
    assert(read && "Unhandled memory read");
    return data;
}
