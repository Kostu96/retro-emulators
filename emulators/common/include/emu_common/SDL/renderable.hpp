#pragma once
#include "emu_common/vec2.hpp"

struct SDL_Renderer;

namespace EmuCommon {

	class Renderable
	{
	public:
		Renderable() = default;
		virtual ~Renderable() = default;

		virtual void render(SDL_Renderer* renderer, Vec2f offset = {}) = 0;
	};

} // EmuCommon
