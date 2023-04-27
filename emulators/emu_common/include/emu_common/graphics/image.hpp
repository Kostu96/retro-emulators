#pragma once
#include "emu_common/graphics/transformable.hpp"
#include "emu_common/graphics/renderable.hpp"

struct SDL_Surface;

namespace EmuCommon {

    class SDLImage :
        public Transformable,
        public Renderable
    {
    public:
        SDLImage() = default;
        ~SDLImage();
    
        bool createFromPixels(void* pixels, unsigned int width, unsigned int height);

        void render(SDL_Renderer* renderer, Transform transform = {}) override;
    private:
        SDL_Surface* m_handle = nullptr;
    };

} // namespace EmuCommon
