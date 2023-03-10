#pragma once
#include "emu_common/graphics/transformable.hpp"
#include "emu_common/graphics/renderable.hpp"
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
    LEDGroup(const EmuCommon::SDLFont& font,
             const EmuCommon::SDLTexture& texture,
             u8 count,
             const char** labels,
             float spacing,
             float tripletSpacing = 0,
             u8 spacerLinesCount = 0,
             u8 originAtIndex = 0);

    ~LEDGroup();

    void render(SDL_Renderer* renderer, EmuCommon::Transform transform = {}) override;

    void setStates(u16 states) { m_states = states; }
    const EmuCommon::Vec2f& getSize() const { return m_size; }
private:
    EmuCommon::SDLText* m_labels = nullptr;
    EmuCommon::SDLSprite* m_leds = nullptr;
    EmuCommon::Vec2f m_size;
    u16 m_states = 0;
    u8 m_count;
    u8 m_spacerLinesCount;
};
