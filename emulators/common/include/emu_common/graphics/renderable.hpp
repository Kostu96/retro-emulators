#pragma once
#include "emu_common/vec2.hpp"

struct SDL_Renderer;

namespace EmuCommon {

	struct RenderStates
	{
		Vec2f position{ 0.f, 0.f };
		Vec2f scale{ 1.f, 1.f };
	};

	class Renderable
	{
	public:
		Renderable() = default;
		virtual ~Renderable() = default;

		virtual void render(SDL_Renderer* renderer, const RenderStates& states) = 0;
	};

} // EmuCommon
