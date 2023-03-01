#pragma once
#include "emu_common/vec2.hpp"

namespace EmuCommon {

	class Transformable
	{
	public:
		Transformable() = default;
		virtual ~Transformable() = default;

		void setOrigin(Vec2f origin) { m_origin = origin; }
		void setPosition(Vec2f position) { m_position = position; }
		const Vec2f& getOrigin() const { return m_origin; }
		const Vec2f& getPosition() const { return m_position; }

		void move(Vec2f offset) { m_position += offset; }
	private:
		Vec2f m_origin;
		Vec2f m_position;
	};

} // namespace EmuCommon
