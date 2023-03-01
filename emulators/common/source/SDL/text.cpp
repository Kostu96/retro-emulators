#include "emu_common/SDL/text.hpp"
#include "emu_common/SDL/font.hpp"

#include <SDL.h>
#include <SDL_ttf.h>
#include <cassert>

namespace EmuCommon {

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

    void SDLText::render(SDL_Renderer* renderer, Vec2f offset)
    {
        if (getSize().x == 0)
            return;

        if (m_isTextureDirty) {
            m_font.setSize(m_textSize);
            SDL_Surface* surface = TTF_RenderUTF8_Blended(m_font, m_text.c_str(), { m_color.r, m_color.g, m_color.b });
            if (surface == nullptr)
                return;

            assert(getSize().x == surface->w);
            assert(getSize().y == surface->h);

            if (m_texture)
                SDL_DestroyTexture(m_texture);

            m_texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (m_texture == nullptr)
                return;

            SDL_FreeSurface(surface);

            m_isTextureDirty = false;
        }

        const SDL_FRect rect{
            getPosition().x + offset.x,
            getPosition().y + offset.y,
            (float)getSize().x,
            (float)getSize().y
        };
        SDL_RenderCopyF(renderer, m_texture, nullptr, &rect);
    }

} // namespace EmuCommon