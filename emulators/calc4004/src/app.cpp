#define _USE_MATH_DEFINES
#include "emulator.hpp"

#include <emu/application.hpp>

#include <array>
#include <cmath>

namespace calc4004 {

using emu::Vec2f;
using emu::Color;

constexpr u16 WINDOW_WIDTH = 640;
constexpr u16 WINDOW_HEIGHT = 640;

struct LEDSegmentGeometry {
    static constexpr int max_num_positions = 6;
    static constexpr int max_num_indices = 12;
    int num_vertices;
    int num_indices;
    std::array<Vec2f, max_num_positions> positions;
    std::array<int, max_num_indices> indices;
};

constinit LEDSegmentGeometry a_segment{
    .num_vertices = 6,
    .num_indices = 12,
    .positions = {
        Vec2f{  9.6f, 1.0f },
        Vec2f{ 22.9f, 1.0f },
        Vec2f{ 24.1f, 2.3f },
        Vec2f{ 21.4f, 4.7f },
        Vec2f{ 10.1f, 4.7f },
        Vec2f{  8.0f, 2.6f }
    },
    .indices = {
        0, 1, 2,
        0, 2, 3,
        3, 4, 5,
        3, 5, 0
    }
};

constinit LEDSegmentGeometry b_segment{
    .num_vertices = 5,
    .num_indices = 9,
    .positions = {
        Vec2f{ 24.7f, 3.4f },
        Vec2f{ 26.0f, 4.7f },
        Vec2f{ 23.6f, 17.2f },
        Vec2f{ 20.4f, 15.3f },
        Vec2f{ 22.1f, 5.8f }
    },
    .indices = {
        0, 1, 2,
        0, 2, 3,
        3, 4, 0
    }
};

constinit LEDSegmentGeometry c_segment{
    .num_vertices = 5,
    .num_indices = 9,
    .positions = {
        Vec2f{ 19.5f, 21.1f },
        Vec2f{ 17.8f, 30.1f },
        Vec2f{ 19.8f, 32.4f },
        Vec2f{ 21.3f, 30.6f },
        Vec2f{ 23.7f, 17.9f }
    },
    .indices = {
        0, 1, 2,
        0, 2, 3,
        3, 4, 0
    }
};

constinit LEDSegmentGeometry d_segment{
    .num_vertices = 6,
    .num_indices = 12,
    .positions = {
        Vec2f{  2.6f, 33.0f },
        Vec2f{  3.8f, 34.3f },
        Vec2f{ 17.2f, 34.3f },
        Vec2f{ 18.7f, 32.8f },
        Vec2f{ 16.8f, 30.5f },
        Vec2f{  5.3f, 30.5f }
    },
    .indices = {
        0, 1, 2,
        0, 2, 3,
        3, 4, 5,
        3, 5, 0
    }
};

constinit LEDSegmentGeometry e_segment{
    .num_vertices = 5,
    .num_indices = 9,
    .positions = {
        Vec2f{ 3.1f, 18.1f },
        Vec2f{ 0.7f, 30.7f },
        Vec2f{ 2.0f, 31.9f },
        Vec2f{ 4.5f, 29.5f },
        Vec2f{ 6.4f, 20.2f }
    },
    .indices = {
        0, 1, 2,
        0, 2, 3,
        3, 4, 0
    }
};

constinit LEDSegmentGeometry f_segment{
    .num_vertices = 5,
    .num_indices = 9,
    .positions = {
        Vec2f{ 5.5f,  4.8f },
        Vec2f{ 3.1f, 17.1f },
        Vec2f{ 7.3f, 14.9f },
        Vec2f{ 8.9f,  4.7f },
        Vec2f{ 7.2f,  2.6f }
    },
    .indices = {
        0, 1, 2,
        0, 2, 3,
        3, 4, 0
    }
};

constinit LEDSegmentGeometry g_segment{
    .num_vertices = 6,
    .num_indices = 12,
    .positions = {
        Vec2f{  7.3f, 16.1f },
        Vec2f{  4.4f, 17.7f },
        Vec2f{  7.4f, 19.6f },
        Vec2f{ 19.3f, 19.6f },
        Vec2f{ 22.0f, 17.9f },
        Vec2f{ 19.1f, 16.1f }
    },
    .indices = {
        0, 1, 2,
        0, 2, 3,
        3, 4, 5,
        3, 5, 0
    }
};

struct LEDDotGeometry {
    static constexpr int num_ring_positions = 12;
    std::array<Vec2f, num_ring_positions + 1> positions;
    std::array<int, num_ring_positions * 3> indices;
};

static constexpr LEDDotGeometry make_dot(Vec2f center, float radius) {
    LEDDotGeometry dot{};
    dot.positions[0] = center;
    constexpr float step = 2.f * to_f32(M_PI) / LEDDotGeometry::num_ring_positions;
    for (int x = 0; x < LEDDotGeometry::num_ring_positions; x++) {
        float alpha = x * step;
        dot.positions[x + 1] = {
            center.x + std::cos(alpha) * radius,
            center.y + std::sin(alpha) * radius
        };
    }
    for (int x = 1; x <= LEDDotGeometry::num_ring_positions; x++) {
        int i1 = x;
        int i2 = (x < LEDDotGeometry::num_ring_positions) ? (x + 1) : 1;
        dot.indices[(x - 1) * 3 + 0] = 0;
        dot.indices[(x - 1) * 3 + 1] = i1;
        dot.indices[(x - 1) * 3 + 2] = i2;
    }
    return dot;
}

LEDDotGeometry dot = make_dot({ 24.f, 32.8f }, 2.f);

class App :
    public emu::Application {
public:
    App() :
        Application(Application::Properties{
            .window_width = WINDOW_WIDTH,
            .window_height = WINDOW_HEIGHT
        }) { }
protected:
    void on_update(s64 delta_time) override {
        time_accumulator_ += delta_time;
        
        while (time_accumulator_ >= calc4004::INSTRUCTION_TIME) {
            time_accumulator_ -= calc4004::INSTRUCTION_TIME;
            emulator_.clock();
        }

        const emu::Renderer2D& renderer = getRenderer();

        constexpr int num_leds = 12;
        constexpr int display_margin = 21; // TODO(Kostu): fix this to account for padding
        constexpr int led_width = 48;
        constexpr int led_height = 60;
        constexpr int display_spacing = 2;
        constexpr int display_padding = 8;

        // display background
        renderer.fill_rect(
            Vec2f{ display_margin - display_padding, display_margin - display_padding },
            Vec2f{ led_width * num_leds + display_spacing * (num_leds - 1) + display_padding * 2,
                   led_height + display_padding * 2
            },
            Color{ 10, 40, 50, 255 }
        );

        Vec2f offset = { 23.f, 23.f };
        float scale = 1.6f;

        for (int i = 0; i < num_leds; i++) {
#if 0 // debug display outline
            renderer.fill_rect(
                Vec2f{ to_f32(display_margin + i * (led_width + display_spacing)), display_margin },
                Vec2f{ led_width, led_height },
                Color{ 10, 220, 40, 255 },
                true
            );
#endif
            renderer.fill_geometry(a_segment.positions, a_segment.indices, offset, scale, Color::Red);
            renderer.fill_geometry(b_segment.positions, b_segment.indices, offset, scale, Color::Red);
            renderer.fill_geometry(c_segment.positions, c_segment.indices, offset, scale, Color::Red);
            renderer.fill_geometry(d_segment.positions, d_segment.indices, offset, scale, Color::Red);
            renderer.fill_geometry(e_segment.positions, e_segment.indices, offset, scale, Color::Red);
            renderer.fill_geometry(f_segment.positions, f_segment.indices, offset, scale, Color::Red);
            renderer.fill_geometry(g_segment.positions, g_segment.indices, offset, scale, Color::Red);
            renderer.fill_geometry(dot.positions, dot.indices, offset, scale, Color::Red);

            offset.x += led_width + display_spacing;
        }
    }
private:
    calc4004::Emulator emulator_;
    s64 time_accumulator_ = 0; // nanoseconds
};

} // namespace calc4004

emu::Application* create_application() {
    return new calc4004::App;
}
