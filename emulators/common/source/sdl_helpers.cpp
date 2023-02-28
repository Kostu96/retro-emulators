#include "emu_common/sdl_helpers.hpp"
#include "emu_common/application.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <cassert>
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

        m_size.x = surface->w;
        m_size.y = surface->h;

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

    bool SDLFont::loadFromFile(const char* filename)
    {
        m_handle = TTF_OpenFont(filename, DEFAULT_SIZE);
        if (m_handle == nullptr) {
            std::cerr << "Could not load font file: " << filename << " SDL_ttf Error: " << TTF_GetError();
            return false;
        }

        TTF_SetFontHinting(m_handle, TTF_HINTING_LIGHT_SUBPIXEL);
        return true;
    }

    void SDLFont::setSize(int size)
    {
        assert(m_handle != nullptr);
        TTF_SetFontSize(m_handle, size);
    }

    SDLText::~SDLText()
    {
        if (m_texture)
            SDL_DestroyTexture(m_texture);
    }

    void SDLText::setText(const char* text)
    {
        m_text = text;
        TTF_SizeUTF8(m_font, m_text.c_str(), &m_size.x, &m_size.y);
        m_isTextureDirty = true;
    }

    void SDLText::setTextSize(unsigned int size)
    {
        m_textSize = size;
        m_font.setSize(m_textSize);
        TTF_SizeUTF8(m_font, m_text.c_str(), &m_size.x, &m_size.y);
        m_isTextureDirty = true;
    }

    void SDLText::render(SDL_Renderer* renderer)
    {
        if (m_isTextureDirty) {
            m_font.setSize(m_textSize);
            SDL_Surface* surface = TTF_RenderUTF8_Blended(m_font, m_text.c_str(), { m_color.r, m_color.g, m_color.b });
            if (surface == nullptr) {
                std::cerr << "Could not render text to surface. SDL_ttf Error: " << TTF_GetError();
                return;
            }
            assert(m_size.x == surface->w);
            assert(m_size.y == surface->h);

            if (m_texture)
                SDL_DestroyTexture(m_texture);

            m_texture = SDL_CreateTextureFromSurface(Application::get().getSDLRenderer(), surface);
            if (m_texture == nullptr) {
                std::cerr << "Could not create texture from text surface. SDL Error: " << SDL_GetError();
                return;
            }

            SDL_FreeSurface(surface);
        }

        SDL_Rect rect = { m_position.x, m_position.y, m_size.x, m_size.y };
        SDL_RenderCopy(renderer, m_texture, nullptr, &rect);
    }

} // EmuCommon
