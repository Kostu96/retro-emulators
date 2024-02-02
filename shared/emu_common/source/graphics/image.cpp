#include "emu_common/graphics/image.hpp"
#include "emu_common/graphics/texture.hpp"

#include <SDL.h>

namespace EmuCommon {

    SDLImage::~SDLImage()
    {
        SDL_FreeSurface(m_handle);
    }

    bool SDLImage::createFromPixels(void* pixels, unsigned int width, unsigned int height)
    {
        m_handle = SDL_CreateRGBSurfaceWithFormatFrom(pixels, width, height, 32, 4 * width, SDL_PIXELFORMAT_RGBA8888);
        if (!m_handle)
            return false;

        return true;
    }

    void SDLImage::render(SDL_Renderer* renderer, Transform transform)
    {
        SDLTexture texture;
        texture.recreateFromSurface(m_handle);
        
        transform *= getTransform();
        FRect rect = transform.tranformRect({ 0, 0, (float)m_handle->w, (float)m_handle->h });
        const SDL_Rect iRect{ (int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height };
        const SDL_Rect textureRect{ 0, 0, m_handle->w, m_handle->h };
        SDL_RenderCopy(renderer, texture.getHandle(), &textureRect, &iRect);
    }

} // namespace EmuCommon
