#pragma once
#include "../shared/type_aliases.hpp"

#include <glw/glw.hpp>

namespace Renderer {

	void init(u16 frameWidth, u16 frameHeight, glw::GLWLoadFunc loadFunc);
	void shutdown();
}
