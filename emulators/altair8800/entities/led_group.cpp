#pragma once
#include "led_group.hpp"
#include "../constants.hpp"

#include <SDL.h>

LEDGroup::LEDGroup(const EmuCommon::SDLFont& font,
                   const EmuCommon::SDLTexture& texture,
                   uint8_t count,
                   const char** labels,
                   float spacing,
                   float tripletSpacing,
                   uint8_t spacerLinesCount,
                   uint8_t originAtIndex) :
    m_count{ count },
    m_spacerLinesCount{ spacerLinesCount }
{
    auto textureSize = texture.getSize();

    m_labels = new EmuCommon::SDLText[count];
    m_leds = new EmuCommon::SDLSprite[count];

    float extent = 0;
    float xPos = 0;
    for (unsigned int i = 0; i < count; i++)
    {
        m_labels[i].setFont(font);
        m_labels[i].setText(labels[i]);
        m_labels[i].setColor(ALTAIR_WHITE_COLOR);
        auto labelSize = m_labels[i].getSize();
        m_labels[i].setOrigin({ labelSize.x / 2.f, 0 });
            
        if (i == 0) {
            extent = std::max((textureSize.x * LED_SPRITE_SCALE) / 4.f, labelSize.x / 2.f);
            xPos = extent;
        }

        m_labels[i].setPosition({ xPos, 0 });
            
        m_leds[i].setTexture(texture);
        m_leds[i].setTextureRect({ 0, 0, int(textureSize.x / 2), int(textureSize.y) });
        m_leds[i].setOrigin({ textureSize.x / 4.f, textureSize.y / 2.f });
        m_leds[i].setPosition({ xPos,
            (textureSize.y * LED_SPRITE_SCALE) / 2.f + labelSize.y + 2.f });
        m_leds[i].setScale({ LED_SPRITE_SCALE, LED_SPRITE_SCALE });

        xPos += spacing + ((count - 1 - i) % 3 == 0 ? tripletSpacing : 0);
    }

    m_size.x = m_leds[count - 1].getPosition().x + extent;
    m_size.y = m_leds[0].getPosition().y + (textureSize.y * LED_SPRITE_SCALE) / 2.f;

    setOrigin({ m_leds[originAtIndex].getPosition().x, 0 });
}

LEDGroup::~LEDGroup()
{
    delete[] m_labels;
    delete[] m_leds;
}

void LEDGroup::render(SDL_Renderer* renderer, EmuCommon::Transform transform)
{
    transform *= getTransform();

#if 0 // DEBUG
    EmuCommon::FRect rect = transform.tranformRect({0, 0, m_size.x, m_size.y });
    SDL_SetRenderDrawColor(renderer, 200, 255, 220, 255);
    SDL_RenderDrawRectF(renderer, reinterpret_cast<SDL_FRect*>(&rect));
#endif

    for (uint8_t i = 0; i < m_count; i++)
    {
        uint8_t bit = m_count - i - 1;
        uint8_t isOn = (m_states >> bit) & 1;
        EmuCommon::IRect rect = m_leds[i].getTextureRect();
        rect.x = isOn * 100; // TODO: remove magic number
        m_leds[i].setTextureRect(rect);
        m_leds[i].render(renderer, transform);
        m_labels[i].render(renderer, transform);
    }

    for (unsigned int i = 0; i < m_count; i++)
        for (unsigned int j = 0; j < m_spacerLinesCount; j++)
        {
            EmuCommon::FRect rect = transform.tranformRect({
                m_leds[i].getPosition().x, m_leds[i].getPosition().y + 18 + j * 16,
                2, 8
                });
            SDL_SetRenderDrawColor(renderer, ALTAIR_WHITE_COLOR.r, ALTAIR_WHITE_COLOR.g, ALTAIR_WHITE_COLOR.b, ALTAIR_WHITE_COLOR.a);
            SDL_RenderDrawRectF(renderer, reinterpret_cast<SDL_FRect*>(&rect));
        }
}
