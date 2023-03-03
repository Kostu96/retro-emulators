#pragma once
#include "../constants.hpp"

#include "emu_common/graphics/transformable.hpp"
#include "emu_common/graphics/renderable.hpp"
#include "emu_common/graphics/rect_shape.hpp"
#include "emu_common/graphics/text.hpp"

class Logo :
    public EmuCommon::Transformable,
    public EmuCommon::Renderable
{
public:
    explicit Logo(const EmuCommon::SDLFont& font) :
        m_size{ float(WINDOW_WIDTH + 2 * ALTAIR_OUTLINE_SIZE), 72.f },
        m_background{ m_size, ALTAIR_WHITE_COLOR },
        m_text1{ font, "ALTAIR 8800" },
        m_text2{ font, "COMPUTER" }
    {
        m_text1.setCharacterSize(64);
        m_text1.setColor(EmuCommon::Color::BLACK);
        m_text1.setPosition({ 64, 0 });
        m_text2.setCharacterSize(46);
        m_text2.setColor(EmuCommon::Color::BLACK);
        m_text2.setPosition({ 385, 10 });
    }

    void render(SDL_Renderer* renderer, const EmuCommon::RenderStates& states = {})
    {
        EmuCommon::RenderStates statesCopy{ states };
        statesCopy.position += getPosition();

        m_background.render(renderer, statesCopy);
        m_text1.render(renderer, statesCopy);
        m_text2.render(renderer, statesCopy);
    }

    const EmuCommon::Vec2f& getSize() const { return m_size; }
private:
    EmuCommon::Vec2f m_size;
    EmuCommon::RectShape m_background;
    EmuCommon::SDLText m_text1;
    EmuCommon::SDLText m_text2;
};
