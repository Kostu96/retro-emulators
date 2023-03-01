#pragma once
#include "widgets/button.hpp"
#include "emu_common/sdl_helpers.hpp"

using EmuCommon::Vec2i;

class LabeledLED {
public:
    LabeledLED(const char* text, EmuCommon::SDLFont& font, EmuCommon::SDLTexture& ledTexture) :
        m_label{ font, text },
        m_LEDTexture{ ledTexture } {
        m_label.setColor({ 0xF2, 0xF1, 0xED });

        m_size.x = std::max(m_label.getSize().x, (m_LEDTexture.getSize().x / 2) / LED_SCALE);
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
        m_upLabel{ font, upText },
        m_upButton{ { 0, 0, 40, int(texture.getSize().y / SWITCH_SCALE) / 2 }},
        m_downLabel{ font, downText },
        m_downButton{ { 0, m_upButton.getSize().y, 40, int(texture.getSize().y / SWITCH_SCALE) / 2}},
        m_texture{ texture },
        m_position{ position } {
        m_upLabel.setColor({ 0xF2, 0xF1, 0xED }); m_upLabel.setPosition({});
        m_downLabel.setColor({ 0xF2, 0xF1, 0xED }); m_downLabel.setPosition({});

        /*m_stopButton.setPosition({ 200, 150 });
        m_stopButton.setPressedCallback([this]() { m_stopRunSwitchPos = 1; });
        m_stopButton.setReleasedCallback([this]() { m_stopRunSwitchPos = 0; m_altair.stop(); });

        m_runButton.setPosition({ 200, 200 });
        m_runButton.setPressedCallback([this]() { m_stopRunSwitchPos = 2; });
        m_runButton.setReleasedCallback([this]() { m_stopRunSwitchPos = 0; m_altair.run(); });*/
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
        m_upLabel.render(renderer, { m_position.x, m_position.y });
        m_upButton.render(renderer, { m_position.x, m_position.y });
        m_downLabel.render(renderer, { m_position.x, m_position.y });
        m_downButton.render(renderer, { m_position.x, m_position.y });
        const int width = m_texture.getSize().x / 3;
        const int height = m_texture.getSize().y;
        const SDL_Rect srcRect{ m_switchPos * width, 0, width, height };
        const SDL_FRect dstRect{ (float)m_position.x, (float)m_position.y, width / SWITCH_SCALE, height / SWITCH_SCALE };
        SDL_RenderCopyF(renderer, m_texture, &srcRect, &dstRect);
    }

    void setPosition(Vec2i position) { m_position = position; }
private:
    static constexpr float SWITCH_SCALE = 3.f;

    EmuCommon::SDLText m_upLabel;
    InvisibleButton m_upButton;
    EmuCommon::SDLText m_downLabel;
    InvisibleButton m_downButton;
    EmuCommon::SDLTexture& m_texture;
    int m_switchPos = 0;
    Vec2i m_position;
};
