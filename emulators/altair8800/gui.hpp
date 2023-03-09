#pragma once
#include "emu_common/graphics/texture.hpp"
#include "emu_common/gui/button.hpp"

#include <SDL.h>
#include <algorithm>

using EmuCommon::Vec2i;

class TwoWayButton {
public:
    TwoWayButton(const char* upText, const char* downText, EmuCommon::SDLFont& font, EmuCommon::SDLTexture& texture, Vec2i position = {}) :
        m_texture{ texture },
        m_upLabel{ font, upText },
        m_downLabel{ font, downText },
        m_position{ position }
    {
        m_width = std::max({
            unsigned int((texture.getSize().x / 3) / SWITCH_SCALE),
            m_upLabel.getSize().x,
            m_downLabel.getSize().x,
        }) + 4;
        if (*upText) {
            m_upButton.setPosition({ 0, 0 });
            m_upButton.setSize({ float(m_width), ((texture.getSize().y / 2) / SWITCH_SCALE / 2) + m_upLabel.getSize().y });
        }
        if (*downText) {
            m_downButton.setPosition({ 0, m_upButton.getSize().y + (int((texture.getSize().y / 2) / SWITCH_SCALE) / 2) - 3 });
            m_downButton.setSize({  float(m_width), ((texture.getSize().y / 2) / SWITCH_SCALE / 2) + m_downLabel.getSize().y });
        }

        m_upLabel.setColor({ 0xF2, 0xF1, 0xED });
        m_upLabel.setPosition({ float((m_width - m_upLabel.getSize().x) / 2), 0 });
        m_upLabel.setAlign(EmuCommon::SDLText::Align::Center);
        m_downLabel.setColor({ 0xF2, 0xF1, 0xED });
        m_downLabel.setPosition({
            float((m_width - m_downLabel.getSize().x) / 2),
            m_downButton.getPosition().y + m_downButton.getSize().y - m_downLabel.getSize().y - 2
        });
        m_downLabel.setAlign(EmuCommon::SDLText::Align::Center);
        
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
        EmuCommon::Transform transform;
        transform.translate(EmuCommon::Vec2f{ m_position });

        m_upLabel.render(renderer, transform);
        m_upButton.render(renderer, transform);
        m_downLabel.render(renderer, transform);
        m_downButton.render(renderer, transform);

        const int width = m_texture.getSize().x / 3;
        const float scaledWidth = width / SWITCH_SCALE;
        const int height = m_texture.getSize().y;
        const SDL_Rect srcRect{ m_switchPos * width, 0, width, height };
        const SDL_FRect dstRect{ (float)m_position.x + ((float)m_width - scaledWidth) / 2.f, (float)m_position.y + m_upLabel.getSize().y / 2, scaledWidth, height / SWITCH_SCALE};
        SDL_RenderCopyF(renderer, m_texture.getHandle(), &srcRect, &dstRect);
    }

    void setPosition(Vec2i position) { m_position = position; }
    int getWidth() const { return m_width; }
private:
    static constexpr float SWITCH_SCALE = 3.f;

    EmuCommon::SDLTexture& m_texture;
    EmuCommon::SDLText m_upLabel;
    EmuCommon::SDLText m_downLabel;
    EmuCommon::GUI::InvisibleButton m_upButton;
    EmuCommon::GUI::InvisibleButton m_downButton;
    int m_switchPos = 0;
    int m_width;
    Vec2i m_position;
};
