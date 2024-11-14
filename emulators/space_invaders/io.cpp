#include "io.hpp"

#include <cassert>
#include <cstdio>

u8 IO::read8(u8 port) const
{
    assert(false && "Unhandled io read!");
    return 0xCD;
}

void IO::write8(u8 port, u8 data)
{
    switch (port) {
    case 0x03: 
        printf("Unhadnled sound at port 3 - data: 0x%02X\n", data);
        return;
    case 0x05:
        printf("Unhadnled sound at port 5 - data: 0x%02X\n", data);
        return;
    case 0x06: return;
    }

    assert(false && "Unhandled io write!");
}
