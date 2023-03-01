#pragma once
#include "emu_common/vec2.hpp"

class Widget
{
public:
	virtual ~Widget() = default;

	virtual void render(SDL_Renderer* renderer, EmuCommon::Vec2i offset = {}) = 0;
	virtual void onEvent(SDL_Event& e) = 0;
	virtual void handleMousePos(EmuCommon::Vec2i position) = 0;

	void setPosition(EmuCommon::Vec2i position) { m_position = position; }
	EmuCommon::Vec2i getPosition() const { return m_position; }
protected:
	EmuCommon::Vec2i m_position;
};
