#pragma once
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
    LEDSingle(const EmuCommon::SDLFont& font, const EmuCommon::SDLTexture& texture, const char* label);

    void render(SDL_Renderer* renderer, EmuCommon::Transform transform = {}) override;
private:
    EmuCommon::SDLText m_label;
    EmuCommon::SDLSprite m_led;
    bool m_state = false;
};
