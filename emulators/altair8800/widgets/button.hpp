#pragma once
#include "emu_common/sdl_helpers.hpp"

#include <functional>
#include <string>

class Button
{
public:
    Button(EmuCommon::SDLFont font,const char* text = "") :
        m_text{ font }
    {
        m_text.setText(text);
    }

    void setPressedCallback(std::function<void()> callback) { m_pressedCallback = callback; }
    void setReleasedCallback(std::function<void()> callback) { m_releasedCallback = callback; }

    void render();
    void onEvent(SDL_Event& e);
private:
    EmuCommon::SDLText m_text;
    std::function<void()> m_pressedCallback;
    std::function<void()> m_releasedCallback;
};
