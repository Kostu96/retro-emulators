#include "button.hpp"

namespace calc4004 {

void draw_button(const emu::Renderer2D& renderer, const Button& button, Vec2f offset) {
    renderer.draw_rect({ offset, { 85.f, 56.f } }, Color::White);
    renderer.draw_texture(button.get_font_texture(), offset, button.get_font_rect());
}

} // namespace calc4004
