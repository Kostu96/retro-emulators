#pragma once
#include <utils/types.hpp>

struct SDL_Renderer;
struct SDL_Texture;

namespace emu {

class Renderer2D;

class Texture :
    NonCopyable {
public:
    Texture() = default;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;
private:
    friend class Renderer2D;
    
    Texture(SDL_Renderer* renderer, const char* filename);

    SDL_Texture* handle_ = nullptr;
    u16 width_ = 0;
    u16 height_ = 0;
};

} // namespace emu
