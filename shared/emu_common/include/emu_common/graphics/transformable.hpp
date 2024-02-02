#pragma once
#include "emu_common/vec2.hpp"
#include "emu_common/graphics/transform.hpp"

namespace EmuCommon {

	class Transformable
	{
	public:
		Transformable() = default;
		virtual ~Transformable() = default;

		void setOrigin(Vec2f origin) { m_origin = origin; m_isTransformDirty = true; }
		void setPosition(Vec2f position) { m_position = position; m_isTransformDirty = true; }
		void setScale(Vec2f scale) { m_scale = scale; m_isTransformDirty = true; }
		void setRotation(float angle);
		const Vec2f& getOrigin() const { return m_origin; }
		const Vec2f& getPosition() const { return m_position; }
		const Vec2f& getScale() const { return m_scale; }
		float getRotation() const { return m_rotation; }
		const Transform& getTransform() const;

		void move(Vec2f offset) { m_position += offset; m_isTransformDirty = true; }
		void scale(Vec2f scale) { m_scale.x *= scale.x; m_scale.y *= scale.y; m_isTransformDirty = true; }
		void rotate(float angle);
	private:
		Vec2f m_origin{ 0.f, 0.f };
		Vec2f m_position{ 0.f, 0.f };
		Vec2f m_scale{ 1.f, 1.f };
		float m_rotation{ 0.f };
		mutable Transform m_transform;
		mutable bool m_isTransformDirty{ true };
	};

} // namespace EmuCommon
