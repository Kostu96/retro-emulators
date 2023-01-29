#pragma once
#include "../shared/type_aliases.hpp"

#include <glw/glw.hpp>

namespace Renderer {

	void init(u16 frameWidth, u16 frameHeight, glw::GLWLoadFunc loadFunc);
	void shutdown();
	void beginFrame();
	void renderFrame(s32 x, s32 y, s32 width, s32 height);

	void renderPoint(u16 x, u16 y, u32 color);
}
