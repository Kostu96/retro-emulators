#pragma once
#include <ccl/types.hpp>

class PIA
{
public:
    PIA() = default;

    u8 load8(u16 address) const;
    void store8(u16 address, u8 data);
private:
    union Control
    {
        struct
        {
            u8 Cx1ActiveTransition : 1;
            u8 Cx2ActiveTransition : 1;
            u8 Cx2Direction : 1;
            u8 Cx2Control : 2;
            u8 PortControl : 1;
            u8 Cx1Control : 2;
        };

        u8 byte = 0;
    };

    u8 m_PRA = 0;  // Port A
    u8 m_DDRA;     // Data Direction Register A
    Control m_CRA; // Control Register A
    u8 m_PRB = 0;  // Port B
    u8 m_DDRB;     // Data Direction Register B
    Control m_CRB; // Control Register B
};
