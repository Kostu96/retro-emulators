#pragma once
#include <ccl/types.hpp>

#include <functional>

class PIA
{
public:
    using IRQCallback = std::function<void(bool)>;
    void mapIRQBCallback(IRQCallback callback) { m_IRQB = callback; }

    u8 load8(u16 address) const;
    void store8(u16 address, u8 data);

    void CB1();

    PIA() = default;
private:
    union Control
    {
        struct
        {
            u8 C1IRQEnable : 1;            // 0
            u8 C1ActiveTransition : 1;     // 1
            u8 PortControl : 1;            // 2
            u8 C2IRQEnable : 1;            // 3
            u8 C2ActiveTransition : 1;     // 4
            u8 C2Direction : 1;            // 5
            u8 C2ActiveTransitionFlag : 1; // 6
            u8 C1ActiveTransitionFlag : 1; // 7
        };

        u8 byte = 0;
    };

    u8 m_PRA = 0;    // Port A
    u8 m_DDRA = 0;   // Data Direction Register A
    Control m_CRA{}; // Control Register A
    u8 m_PRB = 0;    // Port B
    u8 m_DDRB = 0;   // Data Direction Register B
    mutable Control m_CRB{}; // Control Register B

    IRQCallback m_IRQB = nullptr;
};
