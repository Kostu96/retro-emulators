#include "emu_common/sdl_helpers.hpp"
#include "emu_common/application.hpp"

#include <SDL.h>
#include <SDL_image.h>

#include <iostream>

namespace EmuCommon {

    SDLTexture::~SDLTexture()
    {
        if (m_handle)
            SDL_DestroyTexture(m_handle);
    }

    bool SDLTexture::loadFromFile(const char* filename)
	{
        SDL_Surface* surface = IMG_Load(filename);
        if (surface == nullptr) {
            std::cerr << "Could not load image file: " << filename << " SDL_image Error: " << IMG_GetError();
            return false;
        }

        m_handle = SDL_CreateTextureFromSurface(Application::get().getSDLRenderer(), surface);
        if (m_handle == nullptr) {
            std::cerr << "Could not create texture from: " << filename << " SDL Error: " << SDL_GetError();
            return false;
        }
            
        SDL_FreeSurface(surface);
        return true;
	}

} // EmuCommon
