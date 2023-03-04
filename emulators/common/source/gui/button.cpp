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

    void TextButton::render(SDL_Renderer* renderer, Transform transform)
    {
        transform *= getTransform();
        FRect rect = transform.tranformRect({ 0, 0, m_size.x, m_size.y });

        SDL_SetRenderDrawColor(renderer, 0x33, 0x33, 0x33, 0xFF);
        SDL_RenderFillRectF(renderer, reinterpret_cast<SDL_FRect*>(&rect));
        
        //statesCopy.position += Vec2f{ 2 * DEFAULT_PADDING, DEFAULT_PADDING }; TODO:
        m_text.render(renderer, transform);
    }

} // namespace EmuCommon::GUI
