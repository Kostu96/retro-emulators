#pragma once
#include "logo.hpp"
#include "../constants.hpp"

Logo::Logo(const EmuCommon::SDLFont& font) :
    m_size{ float(WINDOW_WIDTH - 2 * ALTAIR_OUTLINE_SIZE), 70.f },
    m_background{ m_size, ALTAIR_WHITE_COLOR },
    m_text1{ font, "ALTAIR 8800" },
    m_text2{ font, "COMPUTER" }
{
    m_text1.setCharacterSize(66);
    m_text1.setColor(EmuCommon::Color::BLACK);
    m_text1.setPosition({ 64, 0 });
    m_text2.setCharacterSize(46);
    m_text2.setColor(EmuCommon::Color::BLACK);
    m_text2.setPosition({ 390, 10 });
}

void Logo::render(SDL_Renderer* renderer, EmuCommon::Transform transform)
{
    transform *= getTransform();

    m_background.render(renderer, transform);
    m_text1.render(renderer, transform);
    m_text2.render(renderer, transform);
}
