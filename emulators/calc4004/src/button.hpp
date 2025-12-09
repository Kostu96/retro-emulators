#pragma once
#include <emu/renderer_2d.hpp>

namespace calc4004 {

using emu::Color;
using emu::Vec2u16;
using emu::Vec2f;
using emu::FRect;

class Button :
    utils::NonCopyable {
public:
    Button(const emu::Texture& font_texture, Vec2f glyph_size, Vec2u16 glyph_coord) :
        font_texture_(font_texture),
        font_rect_(
            glyph_coord.x* glyph_size.x, glyph_coord.y* glyph_size.y,
            glyph_size.x, glyph_size.y
        ) {
    }
public:
    const emu::Texture& get_font_texture() const { return font_texture_; }
    FRect get_font_rect() const { return font_rect_; }
private:
    const emu::Texture& font_texture_;
    FRect font_rect_;
};

void draw_button(const emu::Renderer2D& renderer, const Button& button, Vec2f offset);

} // namespace calc4004
