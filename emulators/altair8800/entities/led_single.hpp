#pragma once
#include "../constants.hpp"
#include "emu_common/graphics/transformable.hpp"
#include "emu_common/graphics/renderable.hpp"
#include "emu_common/graphics/sprite.hpp"
#include "emu_common/graphics/text.hpp"

#include <ccl/non_copyable.h>

class LEDSingle :
    public EmuCommon::Transformable,
    public EmuCommon::Renderable,
    public ccl::NonCopyable
{
public:
    LEDSingle(const EmuCommon::SDLFont& font, const EmuCommon::SDLTexture& texture, const char* label) :
        m_label{ font, label },
        m_led{ texture, { 0, 0, int(texture.getSize().x / 2), int(texture.getSize().y) }}
    {
        auto labelSize = m_label.getSize();
        m_label.setColor(ALTAIR_WHITE_COLOR);
        m_label.setOrigin({ labelSize.x / 2.f, 0 });

        m_led.setScale({ LED_SPRITE_SCALE, LED_SPRITE_SCALE });
    }

    void render(SDL_Renderer* renderer, EmuCommon::Transform transform = {}) override
    {
        transform *= getTransform();
        m_label.render(renderer, transform);
        m_led.render(renderer, transform);
    }
private:
    EmuCommon::SDLText m_label;
    EmuCommon::SDLSprite m_led;
    bool m_state = false;
};
