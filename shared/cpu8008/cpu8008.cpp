#include "cpu8008.hpp"

#include <bitset>
#include <cassert>

void CPU8008::reset()
{
    
}

void CPU8008::clock()
{
   /* u8 instruction = load8(PC++);

    switch (instruction)
    {
    
    default:
        assert(false && "Unhandled instruction");
    }*/
}

u8 CPU8008::load8(u16 address) const
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

u16 CPU8008::load16(u16 address)
{
    u16 data = 0;
    bool read = false;
    for (auto& entry : m_readMap)
    {
        u16 offset;
        if (entry.range.contains(address, offset))
        {
            data = entry.read(offset + 1);
            data <<= 8;
            data |= entry.read(offset);
            read = true;
            break;
        }
    }
    assert(read && "Unhandled memory read");
    return data;
}

void CPU8008::store8(u16 address, u8 data)
{
    bool stored = false;
    for (auto& entry : m_writeMap)
    {
        u16 offset;
        if (entry.range.contains(address, offset))
        {
            entry.write(offset, data);
            stored = true;
            break;
        }
    }
    assert(stored && "Unhandled memory write");
}

void CPU8008::store16(u16 address, u16 data)
{
    bool stored = false;
    for (auto& entry : m_writeMap)
    {
        u16 offset;
        if (entry.range.contains(address, offset))
        {
            entry.write(offset, data & 0xFF);
            entry.write(offset + 1, data >> 8);
            stored = true;
            break;
        }
    }
    assert(stored && "Unhandled memory write");
}

void CPU8008::push8(u8 data)
{
    store8(--SP, data);
}

void CPU8008::push16(u16 data)
{
    store16(SP - 2, data);
    SP -= 2;
}

u8 CPU8008::pop8()
{
    return load8(SP++);
}

u16 CPU8008::pop16()
{
    SP += 2;
    return load16(SP - 2);
}
