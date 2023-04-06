#pragma once
#include "emu_common/gui/widget.hpp"
#include "emu_common/graphics/sprite.hpp"

#include <functional>

namespace EmuCommon::GUI {

    class CheckboxBase :
        public Widget
    {
    public:
        using OnStateChangeCallback = std::function<void(bool previousState, bool newState)>;

        CheckboxBase() = default;

        void onEvent(const SDL_Event& e);
        void handleMousePos(Vec2i position);

        void setOnStateChangeCallback(OnStateChangeCallback callback) { m_onStateChangeCallback = callback; }
        Vec2f getSize() const { return m_size; }
        bool getState() const { return m_state; }
    protected:
        OnStateChangeCallback m_onStateChangeCallback;
        Vec2f m_size;
        bool m_isHovered = false;
        bool m_isPressed = false;
        bool m_state = false;
    };

    class ImageCheckbox :
        public CheckboxBase
    {
    public:
        ImageCheckbox() = default;

        void render(SDL_Renderer* renderer, Transform transform = {}) override
        {
            transform *= getTransform();
            m_sprite.render(renderer, transform);
        }

        void setTexture(const SDLTexture& texture, bool resetRect = false);
        void setTextureRect(const IRect& rect);
    private:
        SDLSprite m_sprite;
    };

} // namespace EmuCommon::GUI
