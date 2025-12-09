#pragma once
#include <utils/types.hpp>

#include <span>

struct SDL_Renderer;
struct SDL_Texture;

namespace emu {

struct Color {
    u8 r, g, b, a;

    static const Color Black;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color White;
};

struct Vec2u16 {
    u16 x, y;
};

struct Vec2f {
    float x{}, y{};

    constexpr Vec2f() = default;
    constexpr Vec2f(float x_, float y_) : x(x_), y(y_) {}
};

struct FRect {
    float x{}, y{}, w{}, h{};

    constexpr FRect() = default;
    constexpr FRect(float x_, float y_, float w_, float h_) :
        x(x_), y(y_), w(w_), h(h_) {}
    constexpr FRect(Vec2f position, Vec2f size) :
        x(position.x), y(position.y), w(size.x), h(size.y) {}
};

class Texture;

class Renderer2D :
    utils::NonCopyable
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

    void draw_rect(FRect rect, Color color, bool fill = true) const;
    void draw_texture(const Texture& texture, Vec2f offset, FRect texture_rect = {}) const;
    void draw_geometry(std::span<const Vec2f> positions, std::span<const int> indices, Vec2f offset, float scale, Color color) const;

    Texture create_texture(const char* filename) const;
private:
    SDL_Renderer* renderer_;
    const Properties properties_;
    SDL_Texture* render_texture_ = nullptr;
};

} // namespace emu
