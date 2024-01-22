#pragma once
#include "type_aliases.hpp"

class PPU2C02
{
public:
    void reset();
    void clock();

    u8 read(u16 offset);
    void write(u16 offset, u8 data);

    PPU2C02() = default;
private:
    union Status
    {
        struct
        {
            u8 unused : 5;
            u8 spriteOverflow : 1;
            u8 sproteZeroHit : 1;
            u8 vertivalBlank : 1;
        } fields;
        u8 byte;
    };

    Status m_status;

    u16 m_cycles;
    s16 m_scanlines;
};
