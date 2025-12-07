#include "emu/renderer_2d.hpp"
#include "emu/texture.hpp"

#include <SDL3/SDL.h>

#include <algorithm>
#include <vector>

namespace emu {

const Color Color::Black{ 0, 0, 0, 255 };
const Color Color::Red{ 255, 0, 0, 255 };
const Color Color::Green{ 0, 255, 0, 255 };
const Color Color::Blue{ 0, 0, 255, 255 };
const Color Color::White{ 255, 255, 255, 255 };

Renderer2D::Renderer2D(SDL_Renderer* renderer, const Properties& properties) :
    renderer_(renderer),
    properties_(properties) {
    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);

    SDL_SetRenderLogicalPresentation(renderer_, properties_.window_width, properties_.window_height, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    const u16 render_width = properties_.scale * properties_.window_width;
    const u16 render_height = properties_.scale * properties_.window_height;
    render_texture_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, render_width, render_height);
    SDL_SetTextureScaleMode(render_texture_, SDL_SCALEMODE_LINEAR);
    SDL_SetRenderTarget(renderer_, render_texture_);
    SDL_SetRenderScale(renderer_, properties_.scale, properties_.scale);
}

Renderer2D::~Renderer2D() {
    SDL_DestroyRenderer(renderer_);
}

void Renderer2D::begin_frame(Color clear_color) const {
    SDL_SetRenderTarget(renderer_, render_texture_);
    SDL_SetRenderDrawColor(renderer_, clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    SDL_RenderClear(renderer_);
}

void Renderer2D::end_frame() const {
    SDL_SetRenderTarget(renderer_, nullptr);
    SDL_SetRenderDrawColor(renderer_, 255, 0, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer_);
    const SDL_FRect rect = { 0.f, 0.f, to_f32(properties_.window_width), to_f32(properties_.window_height) };
    SDL_RenderTexture(renderer_, render_texture_, nullptr, &rect);
    SDL_RenderPresent(renderer_);
}

void Renderer2D::draw_rect(FRect rect, Color color, bool fill) const {
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
    const SDL_FRect sdl_rect{
        .x = rect.x, .y = rect.y,
        .w = rect.w, .h = rect.h
    };
    if (fill) {
        SDL_RenderFillRect(renderer_, &sdl_rect);
    }
    else {
        SDL_RenderRect(renderer_, &sdl_rect);
    }
}

void Renderer2D::draw_texture(const Texture& texture, Vec2f offset, FRect texture_rect) const {
    const SDL_FRect src_rect = { texture_rect.x, texture_rect.y, texture_rect.w, texture_rect.h };
    const SDL_FRect dst_rect = {
        offset.x, offset.y,
        texture_rect.w == 0 ? to_f32(texture.width_) : texture_rect.w,
        texture_rect.h == 0 ? to_f32(texture.height_) : texture_rect.h
    };
    SDL_RenderTexture(renderer_, texture.handle_, &src_rect, &dst_rect);
}

void Renderer2D::draw_geometry(std::span<const Vec2f> positions, std::span<const int> indices, Vec2f offset, float scale, Color color) const {
    std::vector<SDL_Vertex> vertices(positions.size());
    std::transform(cbegin(positions), cend(positions), begin(vertices),
        [offset, scale, color](Vec2f position) {
            SDL_Vertex vertex{};
            vertex.position.x = position.x * scale + offset.x;
            vertex.position.y = position.y * scale + offset.y;
            vertex.color.r = color.r / 255.f;
            vertex.color.g = color.g / 255.f;
            vertex.color.b = color.b / 255.f;
            vertex.color.a = color.a / 255.f;
            return vertex;
        }
    );

    SDL_RenderGeometry(renderer_, nullptr, vertices.data(), static_cast<int>(vertices.size()), indices.data(), static_cast<int>(indices.size()));
}

Texture Renderer2D::create_texture(const char* filename) const {
    return Texture(renderer_, filename);
}

}
