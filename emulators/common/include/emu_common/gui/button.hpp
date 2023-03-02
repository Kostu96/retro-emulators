#pragma once
#include "emu_common/gui/widget.hpp"
#include "emu_common/graphics/text.hpp"

#include <functional>

namespace EmuCommon::GUI {

    class ButtonBase :
        public Widget
    {
    public:
        void onEvent(const SDL_Event& e) override;
        void handleMousePos(Vec2i position) override;
        
        void setPressedCallback(std::function<void()> callback) { m_pressedCallback = callback; }
        void setReleasedCallback(std::function<void()> callback) { m_releasedCallback = callback; }
        Vec2f getSize() const { return m_size; }
    protected:
        std::function<void()> m_pressedCallback;
        std::function<void()> m_releasedCallback;
        bool m_isHovered = false;
        bool m_isPressed = false;
        Vec2f m_size;
    };

    class InvisibleButton :
        public ButtonBase
    {
    public:
        explicit InvisibleButton(Vec2f size = {}) { setSize(size); }

        void render(SDL_Renderer* /*renderer*/, const RenderStates& /*states*/) override {}

        void setSize(Vec2f size) { m_size = size; }
    };

    class TextButton :
        public ButtonBase
    {
    public:
        TextButton(const SDLFont& font, const char* text = "") :
            m_text{ font }
        {
            m_text.setText(text);
            m_size.x = float(m_text.getSize().x + 4 * DEFAULT_PADDING);
            m_size.y = float(m_text.getSize().y + 2 * DEFAULT_PADDING);
        }

        void render(SDL_Renderer* renderer, const RenderStates& states) override;
    private:
        static constexpr int DEFAULT_PADDING = 4;

        SDLText m_text;
    };

    //class ImageButton :
    //    public ButtonBase
    //{
    //public:
    //
    //private:
    //
    //};

} // namespace EmuCommon::GUI
