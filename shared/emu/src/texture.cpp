#include "emu/texture.hpp"

#include <third_party/stb/stb_image.h>
#include <SDL3/SDL.h>

#include <memory>

namespace emu {

Texture::Texture(Texture&& other) noexcept :
    handle_(other.handle_),
    width_(other.width_),
    height_(other.height_) {
    other.handle_ = nullptr;
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        SDL_DestroyTexture(handle_);
        handle_ = other.handle_;
        width_ = other.width_;
        height_ = other.height_;
        other.handle_ = nullptr;
    }
    return *this;
}

Texture::Texture(SDL_Renderer* renderer, const char* filename) {
    int width, height, channels;

    std::unique_ptr<stbi_uc, void(*)(void*)> pixels(
        stbi_load(filename, &width, &height, &channels, 4),
        stbi_image_free
    );

    handle_ = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, width, height);
    SDL_UpdateTexture(handle_, nullptr, pixels.get(), width * 4);
    width_ = to_u16(width);
    height_ = to_u16(height);
}

}
