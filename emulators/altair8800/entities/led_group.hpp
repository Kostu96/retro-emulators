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

class LEDGroup :
    public EmuCommon::Transformable,
    public EmuCommon::Renderable,
    public ccl::NonCopyable
{
public:
    LEDGroup(const EmuCommon::SDLFont& font, const EmuCommon::SDLTexture& texture, u8 count, const char** labels, float spacing) :
        m_count{ count }
    {
        m_labels = new EmuCommon::SDLText[count];
        for (unsigned int i = 0; i < count; i++)
        {
            m_labels[i].setFont(font);
            m_labels[i].setText(labels[i]);
            m_labels[i].setColor(ALTAIR_WHITE_COLOR);
        }

        m_leds = new EmuCommon::SDLSprite[count];
        for (unsigned int i = 0; i < count; i++)
        {
            auto size = texture.getSize();
            m_leds[i].setTexture(texture);
            m_leds[i].setTextureRect({ 0, 0, int(size.x / 2), int(size.y) });
            m_leds[i].setOrigin({ size.x / 4.f, size.y / 2.f });
            m_leds[i].setPosition({ i * spacing, 0.f });
        }
    }

    ~LEDGroup()
    {
        delete[] m_labels;
        delete[] m_leds;
    }

    void render(SDL_Renderer* renderer, EmuCommon::Transform transform = {})
    {
        transform *= getTransform();

        for (unsigned int i = 0; i < m_count; i++)
        {
            m_leds[i].render(renderer, transform);
        }
    }
private:
    EmuCommon::SDLText* m_labels = nullptr;
    EmuCommon::SDLSprite* m_leds = nullptr;
    u16 m_states = 0;
    u8 m_count;
};
