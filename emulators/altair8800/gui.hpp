#pragma once
#include "widgets/button.hpp"
#include "emu_common/sdl_helpers.hpp"

#include <algorithm>

using EmuCommon::Vec2i;

class LabeledLED
{
public:
    LabeledLED(const char* text, EmuCommon::SDLFont& font, EmuCommon::SDLTexture& ledTexture) :
        m_label{ font, text },
        m_LEDTexture{ ledTexture } {
        m_label.setColor({ 0xF2, 0xF1, 0xED });

        m_size.x = std::max(m_label.getSize().x, int(m_LEDTexture.getSize().x / 2) / LED_SCALE);
        m_size.y = m_label.getSize().y + m_LEDTexture.getSize().y / LED_SCALE + 2;
    }

    void setPosition(Vec2i position) {
        m_position = position;
        int xPos = position.x + ((m_size.x == m_label.getSize().x) ? 0 : (m_size.x - m_label.getSize().x) / 2);
        m_label.setPosition({ xPos, position.y });
    }

    Vec2i getSize() const { return m_size; }

    void render(SDL_Renderer* renderer) {
        m_label.render(renderer);
        const int width = m_LEDTexture.getSize().x / 2;
        const int scaledWidth = width / LED_SCALE;
        const int height = m_LEDTexture.getSize().y;
        const SDL_Rect srcRect{ m_status ? width : 0, 0, width, height };
        int xPos = m_position.x + ((m_size.x == scaledWidth) ? 0 : (m_size.x - scaledWidth) / 2);
        const SDL_Rect dstRect{ xPos, m_position.y + m_label.getSize().y + 2, scaledWidth, height / LED_SCALE };
        SDL_RenderCopy(renderer, m_LEDTexture, &srcRect, &dstRect);
    }
private:
    static constexpr int LED_SCALE = 4;

    bool m_status = false;
    Vec2i m_position;
    Vec2i m_size;
    EmuCommon::SDLText m_label;
    EmuCommon::SDLTexture& m_LEDTexture;
};

class TwoWayButton {
public:
    TwoWayButton(const char* upText, const char* downText, EmuCommon::SDLFont& font, EmuCommon::SDLTexture& texture, Vec2i position = {}) :
        m_texture{ texture },
        m_upLabel{ font, upText },
        m_downLabel{ font, downText },
        m_secondLabel{ font },
        m_position{ position }
    {
        int upBtnOff = 0;
        std::string str{ upText };
        size_t pos = str.find_first_of('\n');
        m_is2line = pos != str.npos;
        if (m_is2line) {
            m_secondLabel.setText(str.substr(pos + 1).c_str());
            str[pos] = '\0';
            m_upLabel.setText(str.c_str());
            upBtnOff = m_secondLabel.getSize().y;
        }

        int downBtnOff = 0;
        str = downText;
        pos = str.find_first_of('\n');
        m_is2line = pos != str.npos;
        if (m_is2line) {
            m_secondLabel.setText(str.substr(pos + 1).c_str());
            str[pos] = '\0';
            m_downLabel.setText(str.c_str());
            downBtnOff = m_secondLabel.getSize().y;
        }

        m_width = std::max({ int((texture.getSize().x / 3) / SWITCH_SCALE), m_upLabel.getSize().x, m_downLabel.getSize().x, m_secondLabel.getSize().x }) + 4;
        if (*upText)
            m_upButton.setRect({ 0, -upBtnOff,
                m_width, (int((texture.getSize().y / 2) / SWITCH_SCALE) / 2) + m_upLabel.getSize().y + upBtnOff
            });
        if (*downText)
            m_downButton.setRect({ 0, m_upButton.getSize().y + (int((texture.getSize().y / 2) / SWITCH_SCALE) / 2) - 3,
                m_width, (int((texture.getSize().y / 2) / SWITCH_SCALE) / 2) + m_downLabel.getSize().y + downBtnOff
            });

        m_upLabel.setColor({ 0xF2, 0xF1, 0xED });
        m_upLabel.setPosition({ (m_width - m_upLabel.getSize().x) / 2, -upBtnOff });
        m_downLabel.setColor({ 0xF2, 0xF1, 0xED });
        m_downLabel.setPosition({
            (m_width - m_downLabel.getSize().x) / 2,
            m_downButton.getPosition().y + m_downButton.getSize().y - m_downLabel.getSize().y - downBtnOff - 2
        });
        m_secondLabel.setColor({ 0xF2, 0xF1, 0xED });
        m_secondLabel.setPosition({ (m_width - m_secondLabel.getSize().x) / 2, downBtnOff ? m_downButton.getPosition().y + m_downButton.getSize().y - m_downLabel.getSize().y - 2 : 0 });

        m_upButton.setPressedCallback([this]() { m_switchPos = 1; });
        m_upButton.setReleasedCallback([this]() { m_switchPos = 0; });

        m_downButton.setPressedCallback([this]() { m_switchPos = 2; });
        m_downButton.setReleasedCallback([this]() { m_switchPos = 0; });
    }

    void onEvent(SDL_Event& e) {
        m_upButton.onEvent(e);
        m_downButton.onEvent(e);
    }

    void handleMousePos(EmuCommon::Vec2i position) {
        m_upButton.handleMousePos({ position.x - m_position.x, position.y - m_position.y });
        m_downButton.handleMousePos({ position.x - m_position.x, position.y - m_position.y });
    }

    void render(SDL_Renderer* renderer) {
        m_upLabel.render(renderer, m_position);
        m_upButton.render(renderer, m_position);
        m_secondLabel.render(renderer, m_position);
        m_downLabel.render(renderer, m_position);
        m_downButton.render(renderer, m_position);
        const int width = m_texture.getSize().x / 3;
        const float scaledWidth = width / SWITCH_SCALE;
        const int height = m_texture.getSize().y;
        const SDL_Rect srcRect{ m_switchPos * width, 0, width, height };
        const SDL_FRect dstRect{ (float)m_position.x + ((float)m_width - scaledWidth) / 2.f, (float)m_position.y + m_upLabel.getSize().y / 2, scaledWidth, height / SWITCH_SCALE};
        SDL_RenderCopyF(renderer, m_texture, &srcRect, &dstRect);
    }

    void setPosition(Vec2i position) { m_position = position; }
    int getWidth() const { return m_width; }
private:
    static constexpr float SWITCH_SCALE = 3.f;

    EmuCommon::SDLTexture& m_texture;
    EmuCommon::SDLText m_upLabel;
    EmuCommon::SDLText m_downLabel;
    EmuCommon::SDLText m_secondLabel;
    int m_width;
    InvisibleButton m_upButton;
    InvisibleButton m_downButton;
    int m_switchPos = 0;
    Vec2i m_position;
    bool m_is2line;
};

class LabeledSwitch
{
public:
    void onEvent(SDL_Event& /*e*/) {

    }

    void handleMousePos(EmuCommon::Vec2i /*position*/) {

    }

    void render(SDL_Renderer* /*renderer*/) {

    }
private:

};