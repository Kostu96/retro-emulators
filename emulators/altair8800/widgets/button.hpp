#pragma once
#include "widget.hpp"

#include <functional>
#include <string>

class ButtonBase :
    public Widget
{
public:
    void setPressedCallback(std::function<void()> callback) { m_pressedCallback = callback; }
    void setReleasedCallback(std::function<void()> callback) { m_releasedCallback = callback; }
    EmuCommon::Vec2i getSize() const { return m_size; }

    void onEvent(SDL_Event& e) override;
    void handleMousePos(EmuCommon::Vec2i position) override;
protected:
    std::function<void()> m_pressedCallback;
    std::function<void()> m_releasedCallback;
    bool m_isHovered = false;
    bool m_isPressed = false;
    EmuCommon::Vec2i m_size;
};

class InvisibleButton :
    public ButtonBase
{
public:
    InvisibleButton(const SDL_Rect& rect = {}) { setRect(rect); }

    void render(SDL_Renderer* renderer, EmuCommon::Vec2i offset = { 0, 0 }) override;

    void setRect(const SDL_Rect& rect);
};

class TextButton :
    public ButtonBase
{
public:
    TextButton(EmuCommon::SDLFont& font, const char* text = "") :
        m_text{ font }
    {
        m_text.setText(text);
        m_size.x = m_text.getSize().x + 4 * DEFAULT_PADDING;
        m_size.y = m_text.getSize().y + 2 * DEFAULT_PADDING;
    }

    void render(SDL_Renderer* renderer, EmuCommon::Vec2i offset = { 0, 0 }) override;
private:
    static constexpr int DEFAULT_PADDING = 4;

    EmuCommon::SDLText m_text;
};

//class ImageButton :
//    public ButtonBase
//{
//public:
//
//private:
//
//};
