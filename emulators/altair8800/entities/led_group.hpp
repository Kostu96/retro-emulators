#pragma once
#include "../constants.hpp"
#include "emu_common/graphics/transformable.hpp"
#include "emu_common/graphics/renderable.hpp"
#include "emu_common/graphics/texture.hpp"
#include "emu_common/graphics/font.hpp"
#include "emu_common/graphics/sprite.hpp"
#include "emu_common/graphics/text.hpp"

#include <ccl/non_copyable.h>
#include <ccl/types.hpp>
#include <SDL.h>

class LEDGroup :
    public EmuCommon::Transformable,
    public EmuCommon::Renderable,
    public ccl::NonCopyable
{
public:
    LEDGroup(const EmuCommon::SDLFont& font, const EmuCommon::SDLTexture& texture,
             u8 count, const char** labels, float spacing, float tripletSpacing = 0) :
        m_count{ count }
    {
        constexpr float textureScale = 0.25f;
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
                extent = std::max((textureSize.x * textureScale) / 4.f, labelSize.x / 2.f);
                xPos = extent;
            }

            m_labels[i].setPosition({ xPos, 0 });
            
            m_leds[i].setTexture(texture);
            m_leds[i].setTextureRect({ 0, 0, int(textureSize.x / 2), int(textureSize.y) });
            m_leds[i].setOrigin({ textureSize.x / 4.f, textureSize.y / 2.f });
            m_leds[i].setPosition({ xPos,
                (textureSize.y * textureScale) / 2.f + labelSize.y + 2.f });
            m_leds[i].setScale({ textureScale, textureScale });

            xPos += spacing + ((count - 1 - i) % 3 == 0 ? tripletSpacing : 0);
        }

        m_size.x = m_leds[count - 1].getPosition().x + extent;
        m_size.y = m_leds[0].getPosition().y + (textureSize.y * textureScale) / 2.f;
    }

    ~LEDGroup()
    {
        delete[] m_labels;
        delete[] m_leds;
    }

    void render(SDL_Renderer* renderer, EmuCommon::Transform transform = {})
    {
        transform *= getTransform();

        EmuCommon::FRect rect = transform.tranformRect({0, 0, m_size.x, m_size.y });
        SDL_SetRenderDrawColor(renderer, 200, 255, 220, 255);
        SDL_RenderDrawRectF(renderer, reinterpret_cast<SDL_FRect*>(&rect));

        for (unsigned int i = 0; i < m_count; i++)
        {
            m_leds[i].render(renderer, transform);
            m_labels[i].render(renderer, transform);
        }
    }

    const EmuCommon::Vec2f& getSize() const { return m_size; }
private:
    EmuCommon::SDLText* m_labels = nullptr;
    EmuCommon::SDLSprite* m_leds = nullptr;
    EmuCommon::Vec2f m_size;
    u16 m_states = 0;
    u8 m_count;
};
