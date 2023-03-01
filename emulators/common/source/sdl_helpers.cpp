#include "emu_common/sdl_helpers.hpp"
#include "emu_common/application.hpp"

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
        m_handle = nullptr;
    }

    bool SDLFont::loadFromFile(const char* filename)
    {
        assert(m_handle == nullptr);
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
        [[maybe_unused]] int ret = TTF_SetFontSize(m_handle, size);
        assert(ret == 0);
    }

    SDLText::~SDLText()
    {
        if (m_texture)
            SDL_DestroyTexture(m_texture);
    }

    void SDLText::setText(const char* text)
    {
        m_text = text;
        m_isSizeDirty = true;
        m_isTextureDirty = true;
    }

    void SDLText::setTextSize(unsigned int size)
    {
        m_textSize = size;
        m_isSizeDirty = true;
        m_isTextureDirty = true;
    }

    Vec2i SDLText::getSize()
    {
        if (m_isSizeDirty && !m_text.empty()) {
            m_font.setSize(m_textSize);
            [[maybe_unused]] int ret = TTF_SizeUTF8(m_font, m_text.c_str(), &m_size.x, &m_size.y);
            assert(ret == 0);
            m_isSizeDirty = false;
        }
        return m_size;
    }

    void SDLText::render(SDL_Renderer* renderer, Vec2i offset)
    {
        if (getSize().x == 0)
            return;

        if (m_isTextureDirty) {
            m_font.setSize(m_textSize);
            SDL_Surface* surface = TTF_RenderUTF8_Blended(m_font, m_text.c_str(), { m_color.r, m_color.g, m_color.b });
            if (surface == nullptr) {
                std::cerr << "Could not render text to surface. SDL_ttf Error: " << TTF_GetError() << '\n';
                return;
            }
            assert(getSize().x == surface->w);
            assert(getSize().y == surface->h);

            if (m_texture)
                SDL_DestroyTexture(m_texture);

            m_texture = SDL_CreateTextureFromSurface(Application::get().getSDLRenderer(), surface);
            if (m_texture == nullptr) {
                std::cerr << "Could not create texture from text surface. SDL Error: " << SDL_GetError() << '\n';
                return;
            }

            SDL_FreeSurface(surface);

            m_isTextureDirty = false;
        }


        SDL_Rect rect = { m_position.x + offset.x, m_position.y + offset.y, getSize().x, getSize().y };
        SDL_RenderCopy(renderer, m_texture, nullptr, &rect);
    }

} // EmuCommon
