#pragma once
#include "emu_common/graphics/transformable.hpp"
#include "emu_common/graphics/renderable.hpp"

struct SDL_Renderer;
union SDL_Event;

namespace EmuCommon::GUI {

	class Widget :
		public Transformable,
		public Renderable
	{
	public:
		virtual ~Widget() = default;

		virtual void onEvent(const SDL_Event& e) = 0;
		virtual void handleMousePos(Vec2i position) = 0;
	};

} // namespace EmuCommon::GUI
