#pragma once
#include "emu_common/gui/widget.hpp"
#include "emu_common/graphics/text.hpp"

namespace EmuCommon::GUI {

    class Label :
        public Widget
    {
    public:
        explicit Label(const SDLFont& font, const char* text = "") :
            m_text{ font, text }
        {}

        void render(SDL_Renderer* renderer, Transform transform = {}) override { m_text.render(renderer, transform); }
    private:
        SDLText m_text;
    };

} // namespace EmuCommon::GUI
