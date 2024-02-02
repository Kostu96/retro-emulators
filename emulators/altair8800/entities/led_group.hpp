#pragma once
#include "emu_common/graphics/transformable.hpp"
#include "emu_common/graphics/renderable.hpp"
#include "emu_common/graphics/sprite.hpp"
#include "emu_common/graphics/text.hpp"

class LEDGroup :
    public EmuCommon::Transformable,
    public EmuCommon::Renderable
{
public:
    LEDGroup(const EmuCommon::SDLFont& font,
             const EmuCommon::SDLTexture& texture,
             uint8_t count,
             const char** labels,
             float spacing,
             float tripletSpacing = 0,
             uint8_t spacerLinesCount = 0,
             uint8_t originAtIndex = 0);

    ~LEDGroup();

    void render(SDL_Renderer* renderer, EmuCommon::Transform transform = {}) override;

    void setStates(uint16_t states) { m_states = states; }
    const EmuCommon::Vec2f& getSize() const { return m_size; }

    LEDGroup(const LEDGroup&) = delete;
    LEDGroup& operator=(const LEDGroup&) = delete;
private:
    EmuCommon::SDLText* m_labels = nullptr;
    EmuCommon::SDLSprite* m_leds = nullptr;
    EmuCommon::Vec2f m_size;
    uint16_t m_states = 0;
    uint8_t m_count;
    uint8_t m_spacerLinesCount;
};
