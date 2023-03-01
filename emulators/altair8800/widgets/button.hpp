#pragma once
#include "widget.hpp"

#include <functional>
#include <string>

class InvisibleButton :
    public Widget
{
public:
    InvisibleButton(const SDL_Rect& rect);

    void setPressedCallback(std::function<void()> callback) { m_pressedCallback = callback; }
    void setReleasedCallback(std::function<void()> callback) { m_releasedCallback = callback; }
    EmuCommon::Vec2i getSize() const { return m_size; }

    void render(SDL_Renderer* renderer, EmuCommon::Vec2i offset = { 0, 0 }) override;
    void onEvent(SDL_Event& e) override;
    void handleMousePos(EmuCommon::Vec2i position) override;
protected:
    std::function<void()> m_pressedCallback;
    std::function<void()> m_releasedCallback;
    bool m_isHovered = false;
    bool m_isPressed = false;
    EmuCommon::Vec2i m_size;
};

class TextButton :
    public InvisibleButton
{
public:
    TextButton(EmuCommon::SDLFont& font, const char* text = "") :
        InvisibleButton{ {} },
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
//    public InvisibleButton
//{
//public:
//
//private:
//
//};
