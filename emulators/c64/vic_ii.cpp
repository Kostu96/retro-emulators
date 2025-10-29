#include "vic_ii.hpp"

#include <cassert>

namespace C64 {

    u8 VICII::load8(u16 /*address*/) const
    {
        assert(false);
        return 0;
    }

    void VICII::store8(u16 address, u8 data)
    {
        address &= 0x3F; // mirror VICII adress range

        switch (address)
        {
        case 0x16: m_control_register2 = data; return;

        case 0x1D: m_SpriteX_expansion = data; return;
        case 0x1E:
        case 0x1F: return; // ignore writes to collision registers
        case 0x20: m_border_color = data; return;
        case 0x21: m_bg_color0 = data; return;
        case 0x22: m_bg_color1 = data; return;
        case 0x23: m_bg_color2 = data; return;
        case 0x24: m_bg_color3 = data; return;
        case 0x25: m_sprite_multicolor0 = data; return;
        case 0x26: m_sprite_multicolor1 = data; return;
        case 0x27: m_sprite0_color = data; return;
        case 0x28: m_sprite1_color = data; return;
        case 0x29: m_sprite2_color = data; return;
        case 0x2A: m_sprite3_color = data; return;
        case 0x2B: m_sprite4_color = data; return;
        case 0x2C: m_sprite5_color = data; return;
        case 0x2D: m_sprite6_color = data; return;
        case 0x2E: m_sprite7_color = data; return;
        }

        assert(false);
    }

} // namespace C64
