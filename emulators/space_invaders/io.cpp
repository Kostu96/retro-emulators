#include "io.hpp"

#include <cassert>
#include <cstdio>

u8 IO::read8(u8 port) const
{
    switch (port) {
    case 0x01: return 0x88; // TODO(Kostu): temp except always ones
    case 0x02: return 0x00; // TODO(Kostu): temp
    case 0x03: return (m_shiftRegister >> (8 - m_shiftAmount)) & 0xFF;
    }

    assert(false && "Unhandled io read!");
    return 0xCD;
}

void IO::write8(u8 port, u8 data)
{
    switch (port) {
    case 0x02:
        m_shiftAmount = data;
        return;
    case 0x03: 
        printf("Unhadnled sound at port 3 - data: 0x%02X\n", data);
        return;
    case 0x04:
        m_shiftRegister >>= 8;
        m_shiftRegister &= 0x00FF;
        m_shiftRegister |= (u16)data << 8;
        return;
    case 0x05:
        printf("Unhadnled sound at port 5 - data: 0x%02X\n", data);
        return;
    case 0x06: return;
    }

    assert(false && "Unhandled io write!");
}
