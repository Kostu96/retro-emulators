#pragma once
#include <ccl/types.hpp>

class VICII
{
public:
    VICII() = default;

    u8 load8(u16 address) const;
    void store8(u16 address, u8 data);
private:
    u8 m_control_register2;

    u8 m_SpriteX_expansion;

    u8 m_border_color;
    u8 m_bg_color0;
    u8 m_bg_color1;
    u8 m_bg_color2;
    u8 m_bg_color3;
    u8 m_sprite_multicolor0;
    u8 m_sprite_multicolor1;
    u8 m_sprite0_color;
    u8 m_sprite1_color;
    u8 m_sprite2_color;
    u8 m_sprite3_color;
    u8 m_sprite4_color;
    u8 m_sprite5_color;
    u8 m_sprite6_color;
    u8 m_sprite7_color;
};
