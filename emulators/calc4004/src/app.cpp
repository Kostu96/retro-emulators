#include "emulator.hpp"
#include "button.hpp"
#include "display.hpp"

#include <emu/application.hpp>
#include <emu/renderer_2d.hpp>
#include <emu/texture.hpp>
#include <utils/log.hpp>

#include <array>

namespace calc4004 {

using emu::Vec2f;

constexpr u16 window_width = 640;
constexpr u16 window_height = 640;

constexpr Vec2f button_glyph_size{ 79.f, 56.f };

class App :
    public emu::Application {
public:
    App() :
        Application(Application::Properties{
            .window_width = window_width,
            .window_height = window_height
        }),
        buttons_{
            Button(font_texture_, button_glyph_size, { 4u, 3u }),
            Button(font_texture_, button_glyph_size, { 2u, 3u }),
            Button(font_texture_, button_glyph_size, { 1u, 2u }),
            Button(font_texture_, button_glyph_size, { 0u, 2u }),
            Button(font_texture_, button_glyph_size, { 1u, 3u }),
            Button(font_texture_, button_glyph_size, { 0u, 3u }),
            Button(font_texture_, button_glyph_size, { 0u, 4u }),
            Button(font_texture_, button_glyph_size, { 3u, 4u }),
            Button(font_texture_, button_glyph_size, { 0u, 0u }),
            Button(font_texture_, button_glyph_size, { 0u, 0u }),
            Button(font_texture_, button_glyph_size, { 0u, 0u }),
            Button(font_texture_, button_glyph_size, { 0u, 0u }),
            Button(font_texture_, button_glyph_size, { 0u, 0u }),
            Button(font_texture_, button_glyph_size, { 0u, 0u }),
            Button(font_texture_, button_glyph_size, { 0u, 0u }),
            Button(font_texture_, button_glyph_size, { 0u, 0u }),
            Button(font_texture_, button_glyph_size, { 0u, 0u }),
            Button(font_texture_, button_glyph_size, { 0u, 0u }),
            Button(font_texture_, button_glyph_size, { 0u, 0u }),
            Button(font_texture_, button_glyph_size, { 0u, 0u }),
            Button(font_texture_, button_glyph_size, { 0u, 0u }),
            Button(font_texture_, button_glyph_size, { 0u, 0u }),
            Button(font_texture_, button_glyph_size, { 0u, 0u }),
            Button(font_texture_, button_glyph_size, { 0u, 0u })
        } {
        const emu::Renderer2D& renderer = getRenderer();
        font_texture_ = renderer.create_texture("data/buttons_font.png");
    }
protected:
    void on_update(s64 delta_time) override {
        time_accumulator_ += delta_time;
        
        while (time_accumulator_ >= calc4004::INSTRUCTION_TIME) {
            time_accumulator_ -= calc4004::INSTRUCTION_TIME;
            emulator_.clock();
        }

        const emu::Renderer2D& renderer = getRenderer();

        display_.render(renderer);

        constexpr float keyboard_x_offset = 130.f;
        Vec2f offset = { keyboard_x_offset, 150.f };
        for (int y = 0; y < 6; y++) {
            for (int x = 0; x < 4; x++) {
                draw_button(renderer, buttons_[y * 4 + x], offset);

                offset.x += 100.f;
            }

            offset.x = keyboard_x_offset;
            offset.y += 70.f;
        }
    }
private:
    calc4004::Emulator emulator_;
    s64 time_accumulator_ = 0; // nanoseconds

    Display display_{};
    std::array<Button, 24> buttons_;
    emu::Texture font_texture_;
};

} // namespace calc4004

emu::Application* create_application() {
    utils::log::info("Creating App");
    return new calc4004::App;
}
