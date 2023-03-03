#include "emu_common/graphics/texture.hpp"
#include "emu_common/application.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <cassert>

namespace EmuCommon {

    SDLTexture::~SDLTexture()
    {
        if (m_handle)
            SDL_DestroyTexture(m_handle);
    }

    bool SDLTexture::loadFromFile(const char* filename)
    {
        SDL_Surface* surface = IMG_Load(filename);
        if (surface == nullptr)
            return false;
        
        bool ret = createFromSurface(surface);
        
        SDL_FreeSurface(surface);
        return ret;
    }

    bool SDLTexture::createFromSurface(SDL_Surface* surface)
    {
        assert(m_handle == nullptr);
        m_handle = SDL_CreateTextureFromSurface(Application::get().getSDLRenderer(), surface);
        if (m_handle == nullptr)
            return false;

        m_size.x = surface->w;
        m_size.y = surface->h;
        return true;
    }

} // namespace EmuCommon
