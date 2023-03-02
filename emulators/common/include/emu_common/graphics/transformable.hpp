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
		void setScale(Vec2f scale) { m_scale = scale; }
		const Vec2f& getOrigin() const { return m_origin; }
		const Vec2f& getPosition() const { return m_position; }
		const Vec2f& getScale() const { return m_scale; }

		void move(Vec2f offset) { m_position += offset; }
		void scale(Vec2f scale) { m_scale.x *= scale.x; m_scale.y *= scale.y; }
	private:
		Vec2f m_origin{ 0.f, 0.f };
		Vec2f m_position{ 0.f, 0.f };
		Vec2f m_scale{ 1.f, 1.f };
	};

} // namespace EmuCommon
