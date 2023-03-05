#pragma once
#include "emu_common/gui/widget.hpp"

namespace EmuCommon::GUI{

	class Checkbox :
		public Widget
	{
	public:
		Checkbox() = default;

		void onEvent(const SDL_Event& e);
		void handleMousePos(Vec2i position);
	private:

	};

} // namespace EmuCommon::GUI
