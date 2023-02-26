#include "emu_common/sdl_helpers.hpp"
#include "emu_common/application.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

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

        m_width = surface->w;
        m_height = surface->h;

        m_handle = SDL_CreateTextureFromSurface(Application::get().getSDLRenderer(), surface);
        if (m_handle == nullptr) {
            std::cerr << "Could not create texture from: " << filename << " SDL Error: " << SDL_GetError();
            return false;
        }
            
        SDL_FreeSurface(surface);
        return true;
	}

    SDLFont::~SDLFont()
    {
        TTF_CloseFont(m_handle);
    }

    bool SDLFont::loadFromFile(const char* filename, int size)
    {
        m_handle = TTF_OpenFont(filename, size);
        if (m_handle == nullptr) {
            std::cerr << "Could not load font file: " << filename << " SDL_ttf Error: " << TTF_GetError();
            return false;
        }

        TTF_SetFontHinting(m_handle, TTF_HINTING_LIGHT_SUBPIXEL);

        return true;
    }

    SDLText::~SDLText()
    {
        if (m_texture)
            SDL_DestroyTexture(m_texture);
    }

    void SDLText::setText(const char* str, uint8_t colorR, uint8_t colorG, uint8_t colorB)
    {
        m_text = str;

        SDL_Surface* surface = TTF_RenderText_Blended(m_font, m_text.c_str(), { colorR, colorG, colorB });
        if (surface == nullptr) {
            std::cerr << "Could not render text to surface. SDL_ttf Error: " << TTF_GetError();
            return;
        }

        m_textureWidth = surface->w;
        m_textureHeight = surface->h;

        if (m_texture)
            SDL_DestroyTexture(m_texture);

        m_texture = SDL_CreateTextureFromSurface(Application::get().getSDLRenderer(), surface);
        if (m_texture == nullptr) {
            std::cerr << "Could not create texture from text surface. SDL Error: " << SDL_GetError();
            return;
        }

        SDL_FreeSurface(surface);
    }

} // EmuCommon
