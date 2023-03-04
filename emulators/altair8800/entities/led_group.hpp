#pragma once
#include "emu_common/graphics/transformable.hpp"
#include "emu_common/graphics/renderable.hpp"

class LEDGroup :
	public EmuCommon::Transformable,
	public EmuCommon::Renderable
{
public:
	LEDGroup() = default;

	void render(SDL_Renderer* /*rednerer*/, EmuCommon::Transform /*transform*/ = {}) {}
private:

};
