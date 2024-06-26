#pragma once
#include "switch_group.hpp"
#include "../constants.hpp"

#include <SDL.h>

SwitchGroup::SwitchGroup(const EmuCommon::SDLFont& font,
                         const EmuCommon::SDLTexture& texture,
                         uint8_t count,
                         const char** labels,
                         float spacing,
                         float tripletSpacing,
                         uint8_t originAtIndex) :
    m_count{ count }
{
    auto textureSize = texture.getSize();

    m_labels = new EmuCommon::SDLText[count];
    m_switches = new EmuCommon::GUI::ImageCheckbox[count];

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
            extent = std::max((textureSize.x * SWITCH_SPRITE_SCALE) / 6.f, labelSize.x / 2.f);
            xPos = extent;
        }

        m_labels[i].setPosition({ xPos, 0 });
            
        m_switches[i].setTexture(texture);
        m_switches[i].setTextureRect({ 2 * int(textureSize.x / 3), 0, int(textureSize.x / 3), int(textureSize.y) });
        m_switches[i].setOrigin({ textureSize.x / 6.f, textureSize.y / 2.f });
        m_switches[i].setPosition({ xPos,
            (textureSize.y * SWITCH_SPRITE_SCALE) / 2.f + labelSize.y - 6.f });
        m_switches[i].setScale({ SWITCH_SPRITE_SCALE, SWITCH_SPRITE_SCALE });
        m_switches[i].setOnStateChangeCallback([this, i, textureSize](bool /*previousState*/, bool newState)
            {
                m_switches[i].setTextureRect({ (newState ? 1 : 2) * int(textureSize.x / 3), 0,
                    int(textureSize.x / 3), int(textureSize.y) });
            }
        );

        xPos += spacing + ((count - 1 - i) % 3 == 0 ? tripletSpacing : 0);
    }

    m_size.x = m_switches[count - 1].getPosition().x + extent;
    m_size.y = m_switches[0].getPosition().y + (textureSize.y * SWITCH_SPRITE_SCALE) / 2.f;

    setOrigin({ m_switches[originAtIndex].getPosition().x, 0 });
}

SwitchGroup::~SwitchGroup()
{
    delete[] m_labels;
    delete[] m_switches;
}

void SwitchGroup::onEvent(SDL_Event& e) {
    for (unsigned int i = 0; i < m_count; i++)
        m_switches[i].onEvent(e);
}

void SwitchGroup::handleMousePos(EmuCommon::Vec2i position)
{
    auto off = getPosition() - getOrigin();
    for (unsigned int i = 0; i < m_count; i++)
        m_switches[i].handleMousePos({ position.x - int(off.x), position.y - int(off.y) });
}

void SwitchGroup::render(SDL_Renderer* renderer, EmuCommon::Transform transform)
{
    transform *= getTransform();

#if 0 // DEBUG
    EmuCommon::FRect rect = transform.tranformRect({0, 0, m_size.x, m_size.y });
    SDL_SetRenderDrawColor(renderer, 200, 255, 220, 255);
    SDL_RenderDrawRectF(renderer, reinterpret_cast<SDL_FRect*>(&rect));
#endif

    for (unsigned int i = 0; i < m_count; i++)
    {
        m_switches[i].render(renderer, transform);
        m_labels[i].render(renderer, transform);
    }
}
