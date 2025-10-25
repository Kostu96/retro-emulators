#pragma once
#include "utils/types.hpp"

namespace C64 {

    class CIA
    {
    public:
        CIA() = default;

        u8 load8(u16 address) const;
        void store8(u16 address, u8 data);
    private:
        u8 m_PRA;  // Data Port A
        u8 m_DDRA; // Data Direction Port A
        u8 m_DDRB; // Data Direction Port B
        u8 m_TAL;  // Timer A Low Byte
        u8 m_TAH;  // Timer A High Byte
        u8 m_ICR;  // Interrupt Control and Status
        u8 m_CRA;  // Control Timer A
        u8 m_CRB;  // Control Timer B
    };

} // namespace C64
