#pragma once
#include "utils/types.hpp"

class VIA6522
{
public:
    VIA6522() = default;

    u8 load8(u16 address) const;
    void store8(u16 address, u8 data);
private:
    u8 m_PRB; // Port B

    u8 m_DDRB; // Data Direction Register B

    u8 m_timer1H; // Timer 1 High Byte

    u8 m_PCR; // Peripheral Control Register

    u8 m_IER; // Interrupt Enable Register
};
