#include "emu_common/graphics/text.hpp"
#include "emu_common/graphics/font.hpp"

#include <SDL.h>
#include <SDL_ttf.h>
#include <cassert>

namespace EmuCommon {

    SDLText::~SDLText()
    {
        destroyTextures();
    }

    void SDLText::setText(const char* text)
    {
        std::string_view str{ text };
        size_t start = 0;
        size_t end = 0;
        do {
            end = str.find_first_of('\n', start);
            m_text.emplace_back(str.substr(start, end - start));
            start = end + 1;
        } while (end != str.npos);

        m_isSizeDirty = true;
        m_isTextureDirty = true;
    }

    void SDLText::setCharacterSize(unsigned int size)
    {
        m_characterSize = size;
        m_isSizeDirty = true;
        m_isTextureDirty = true;
    }

    Vec2u SDLText::getSize()
    {
        if (m_isSizeDirty) {
            m_font.setSize(m_characterSize);

            assert(!m_text.empty());
            int w, h;
            [[maybe_unused]] int ret = TTF_SizeUTF8(m_font.getHandle(), m_text[0].c_str(), &w, &h);
            assert(ret == 0);
            int maxWidth = w;
            m_size.y = h;
            m_lineHeight = (float)h;

            for (auto it = m_text.begin() + 1; it != m_text.end(); it++) {
                ret = TTF_SizeUTF8(m_font.getHandle(), it->c_str(), &w, &h);
                assert(ret == 0);
                if (maxWidth < w) maxWidth = w;
                m_size.y += h;
            }

            m_size.x = maxWidth;
            m_isSizeDirty = false;
        }
        return m_size;
    }

    void SDLText::render(SDL_Renderer* renderer, const RenderStates& states)
    {
        if (getSize().x == 0)
            return;

        if (m_isTextureDirty) {
            destroyTextures();
            m_font.setSize(m_characterSize);

            for (const auto& line : m_text) {
                if (line.empty()) continue;

                SDL_Surface* surface = TTF_RenderUTF8_Blended(m_font.getHandle(), line.c_str(), { m_color.r, m_color.g, m_color.b });
                assert(surface && "Could not render text surface!");

                SDLTexture* texture = new SDLTexture{};
                bool ret = texture->createFromSurface(surface);
                assert(ret && "Could not create texture from text surface!");
                m_textures.push_back(texture);

                SDL_FreeSurface(surface);
            }

            m_isTextureDirty = false;
        }

        SDL_FRect rect{
            getPosition().x + states.position.x,
            getPosition().y + states.position.y,
            (float)getSize().x * getScale().x * states.scale.x,
            (float)getSize().y * getScale().y * states.scale.y
        };
        SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
        SDL_RenderDrawRectF(renderer, &rect);

        assert(!m_textures.empty());
        for (auto& texture : m_textures) {
            rect.w = (float)texture->getSize().x * getScale().x * states.scale.x;
            rect.h = (float)texture->getSize().y * getScale().y * states.scale.y;
            switch (m_align)
            {
            case Align::Left:

                break;
            case Align::Center:
                rect.x = floorf(getPosition().x + states.position.x + (float)(getSize().x - texture->getSize().x) / 2.f);
                break;
            case Align::Right:
                rect.x = ceilf(getPosition().x + states.position.x + (float)(getSize().x - texture->getSize().x));
                break;
            }
            SDL_RenderCopyF(renderer, texture->getHandle(), nullptr, &rect);
            rect.y += m_lineHeight;
        }
    }

    void SDLText::destroyTextures()
    {
        for (auto tex : m_textures)
            delete tex;

        m_textures.clear();
    }

} // namespace EmuCommon
