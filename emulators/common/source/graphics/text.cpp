#include "emu_common/graphics/text.hpp"
#include "emu_common/graphics/font.hpp"

#include <SDL.h>
#include <SDL_ttf.h>
#include <cassert>

namespace EmuCommon {

    void SDLText::setText(const char* text)
    {
        m_text = text;

        size_t start = 0;
        size_t end = 0;
        do {
            end = m_text.find_first_of('\n', start);
            m_lines.emplace_back(m_text.substr(start, end - start));
            start = end + 1;
        } while (end != m_text.npos);

        m_isSizeDirty = true;
        m_isTextureDirty = true;
    }

    void SDLText::setFont(const SDLFont& font)
    {
        m_font = &font;
        m_isSizeDirty = true;
        m_isTextureDirty = true;
    }

    void SDLText::setCharacterSize(unsigned int size)
    {
        m_characterSize = size;
        m_isSizeDirty = true;
        m_isTextureDirty = true;
    }

    void SDLText::setColor(Color color)
    {
        m_color = color;
        m_isTextureDirty = true;
    }

    void SDLText::setAlign(Align align)
    {
        m_align = align;
        m_isTextureDirty = true;
    }

    Vec2u SDLText::getSize()
    {
        if (m_isSizeDirty) {
            m_font->setSize(m_characterSize);

            assert(!m_lines.empty());
            int w, h;
            [[maybe_unused]] int ret = TTF_SizeUTF8(m_font->getHandle(), m_lines[0].c_str(), &w, &h);
            assert(ret == 0);
            int maxWidth = w;
            m_size.y = h;
            m_lineHeight = h;

            for (auto it = m_lines.begin() + 1; it != m_lines.end(); it++) {
                ret = TTF_SizeUTF8(m_font->getHandle(), it->c_str(), &w, &h);
                assert(ret == 0);
                if (maxWidth < w) maxWidth = w;
                m_size.y += h;
            }

            m_size.x = maxWidth;
            m_isSizeDirty = false;
        }
        return m_size;
    }

    void SDLText::render(SDL_Renderer* renderer, Transform transform)
    {
        if (getSize().x == 0)
            return;

        if (m_isTextureDirty) {
            m_font->setSize(m_characterSize);
            SDL_Surface* textSurface = SDL_CreateRGBSurfaceWithFormat(0, getSize().x, getSize().y, 32, SDL_PIXELFORMAT_ARGB32);
            assert(textSurface && "Could not create text surface!");

            SDL_Rect dstRect{ 0, 0, 0, 0 };
            for (const auto& line : m_lines) {
                if (line.empty()) continue;

                SDL_Surface* surface = TTF_RenderUTF8_Blended(m_font->getHandle(), line.c_str(), { m_color.r, m_color.g, m_color.b });
                assert(surface && "Could not render line surface!");
                
                switch (m_align)
                {
                case Align::Center:
                    dstRect.x = (getSize().x - surface->w) / 2;
                    break;
                case Align::Right:
                    dstRect.x = getSize().x - surface->w;
                    break;
                }

                SDL_BlitSurface(surface, nullptr, textSurface, &dstRect);
                dstRect.y += m_lineHeight;

                SDL_FreeSurface(surface);
            }

            m_texture.recreateFromSurface(textSurface);
            SDL_FreeSurface(textSurface);
            m_isTextureDirty = false;
        }

        transform *= getTransform();
        FRect rect = transform.tranformRect({ 0, 0, (float)getSize().x, (float)getSize().y });
        const SDL_Rect iRect{ (int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height };

#if 0 // DEBUG
        SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
        SDL_RenderDrawRect(renderer, &iRect);
#endif

        SDL_RenderCopy(renderer, m_texture.getHandle(), nullptr, &iRect);
    }

} // namespace EmuCommon
