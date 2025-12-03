#pragma once
#include <utils/types.hpp>

#include <span>

struct SDL_Renderer;
struct SDL_Texture;

namespace emu {

// TODO(Kostu): move this somewhere
struct Color {
    u8 r, g, b, a;

    static const Color Black;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color White;
};

struct Vec2f {
    float x, y;

    constexpr Vec2f() = default;
    constexpr Vec2f(float x_, float y_) : x(x_), y(y_) {}
};

class Renderer2D :
    NonCopyable
{
public:
    struct Properties {
        u16 window_width;
        u16 window_height;
        u16 scale;
    };

    Renderer2D(SDL_Renderer* renderer, const Properties& properties);
    ~Renderer2D();

    void begin_frame(Color clear_color = { 0, 0, 0, 255 }) const;
    void end_frame() const;

    void fill_rect(Vec2f position, Vec2f size, Color color, bool lines_only = false) const;
    void fill_geometry(std::span<const Vec2f> positions, std::span<const int> indices, Vec2f offset, float scale, Color color) const;
private:
    SDL_Renderer* renderer_;
    const Properties properties_;
    SDL_Texture* render_texture_ = nullptr;
};

} // namespace emu
