#include "button.hpp"
#include "emu_common/application.hpp"

InvisibleButton::InvisibleButton(const SDL_Rect& rect)
{
    m_position.x = rect.x;
    m_position.y = rect.y;
    m_size.x = rect.w;
    m_size.y = rect.h;
}

void InvisibleButton::render(SDL_Renderer* renderer, EmuCommon::Vec2i offset)
{
    SDL_Rect rect = { m_position.x + offset.x, m_position.y + offset.y, m_size.x, m_size.y };
    SDL_SetRenderDrawColor(renderer, 0xF2, 0xF1, 0xED, 0xFF);
    SDL_RenderDrawRect(renderer, &rect);
}

void InvisibleButton::onEvent(SDL_Event& e)
{
    switch (e.type)
    {
    case SDL_MOUSEBUTTONDOWN:
        if (m_isHovered) {
            m_isPressed = true;
            if (m_pressedCallback) m_pressedCallback();
        }
        break;
    case SDL_MOUSEBUTTONUP:
        if (m_isPressed) {
            m_isPressed = false;
            if (m_releasedCallback) m_releasedCallback();
        }
        break;
    };
}

void InvisibleButton::handleMousePos(EmuCommon::Vec2i position)
{
    SDL_Point p{ position.x, position.y };
    SDL_Rect r{ m_position.x, m_position.y, m_size.x, m_size.y };
    m_isHovered = SDL_PointInRect(&p, &r);
}

void TextButton::render(SDL_Renderer* renderer, EmuCommon::Vec2i offset)
{
    SDL_Rect rect = { m_position.x + offset.x, m_position.y + offset.y, m_size.x, m_size.y };
    SDL_SetRenderDrawColor(renderer, 0x33, 0x33, 0x33, 0xFF);
    SDL_RenderFillRect(renderer, &rect);
    m_text.render(renderer, { m_position.x + 2 * DEFAULT_PADDING, m_position.y + DEFAULT_PADDING });
}
