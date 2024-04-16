#include "emu_common/gui/checkbox.hpp"
#include "emu_common/application.hpp"
#include "emu_common/graphics/texture.hpp"

#include <SDL.h>

namespace EmuCommon::GUI {

	void CheckboxBase::onEvent(const SDL_Event& e)
	{
        switch (e.type)
        {
        case SDL_MOUSEBUTTONDOWN:
            if (m_isHovered) {
                m_isPressed = true;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (m_isPressed) {
                m_isPressed = false;
                bool previousState = m_state;
                m_state = !m_state;
                if (m_onStateChangeCallback)
                    m_onStateChangeCallback(previousState, m_state);
            }
            break;
        };
	}

	void CheckboxBase::handleMousePos(Vec2i position)
	{
        SDL_FPoint point{ (float)position.x, (float)position.y };
        FRect rect = getTransform().tranformRect({ 0, 0, m_size.x, m_size.y });
        m_isHovered = SDL_PointInFRect(&point, reinterpret_cast<SDL_FRect*>(&rect));

        if (m_isHovered) EmuCommon::Application::get().setHandCursor();
	}

    void ImageCheckbox::setTexture(const SDLTexture& texture, bool resetRect)
    {
        m_sprite.setTexture(texture, resetRect);
        if (resetRect) {
            m_size.x = float(texture.getSize().x);
            m_size.y = float(texture.getSize().y);
        }
    }

    void ImageCheckbox::setTextureRect(const IRect& rect)
    {
        m_sprite.setTextureRect(rect);
        m_size.x = float(rect.width);
        m_size.y = float(rect.height);
    }

} // namespace EmuCommon::GUI
