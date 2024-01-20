#pragma once
#include <ccl/types.hpp>

class PIA
{
public:
    PIA() = default;

    u8 load8(u16 address) const;
    void store8(u16 address, u8 data);
private:
    u8 m_PRA; // Port A
    u8 m_CA;
    u8 m_PRB; // Port B
    u8 m_CB;
};
