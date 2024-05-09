#include "sid.hpp"

#include <cassert>

namespace C64 {

    u8 SID::load8(u16 /*address*/) const
    {
        assert(false);
        return 0;
    }

    void SID::store8(u16 address, u8 data)
    {
        address &= 0xFF; // mirror SID adress range

        switch (address)
        {
        case 0x18: m_filter_mode_and_main_volume_control = data; return;
        }

        assert(false);
    }

} // namespace C64
