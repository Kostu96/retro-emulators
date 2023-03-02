#include "emu_common/gui/button.hpp"
#include "emu_common/application.hpp"

#include <SDL.h>

namespace EmuCommon::GUI {

    void ButtonBase::onEvent(const SDL_Event& e)
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

    void ButtonBase::handleMousePos(Vec2i position)
    {
        SDL_FPoint p{ (float)position.x, (float)position.y };
        SDL_FRect r{ getPosition().x, getPosition().y, m_size.x, m_size.y};
        m_isHovered = SDL_PointInFRect(&p, &r);

        if (m_isHovered) EmuCommon::Application::get().setHandCursor();
    }

    void TextButton::render(SDL_Renderer* renderer, const RenderStates& states)
    {
        RenderStates statesCopy{ states };
        statesCopy.position += getPosition();
        statesCopy.scale.x *= getScale().x;
        statesCopy.scale.y *= getScale().y;

        const SDL_FRect rect{
            statesCopy.position.x,
            statesCopy.position.y,
            m_size.x * statesCopy.scale.x,
            m_size.y * statesCopy.scale.y
        };
        SDL_SetRenderDrawColor(renderer, 0x33, 0x33, 0x33, 0xFF);
        SDL_RenderFillRectF(renderer, &rect);
        
        statesCopy.position += Vec2f{ 2 * DEFAULT_PADDING, DEFAULT_PADDING };
        m_text.render(renderer, statesCopy);
    }

} // namespace EmuCommon::GUI
