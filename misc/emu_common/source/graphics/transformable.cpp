#pragma once
#include "emu_common/graphics/transformable.hpp"

#include <cmath>

static constexpr float  PI_F32 = 3.141592653589;
static constexpr double PI_F64 = 3.14159265358979323;

namespace EmuCommon {

	void Transformable::setRotation(float angle)
	{
		m_rotation = angle;
		m_rotation = fmod(360.f + fmod(m_rotation, 360.f), 360.f);
		m_isTransformDirty = true;
	}

	const Transform& Transformable::getTransform() const
	{
        if (m_isTransformDirty)
        {
            float angle = -m_rotation * PI_F32 / 180.f;
            float cosine = std::cos(angle);
            float sine = std::sin(angle);
            float sxc = m_scale.x * cosine;
            float syc = m_scale.y * cosine;
            float sxs = m_scale.x * sine;
            float sys = m_scale.y * sine;
            float tx = -m_origin.x * sxc - m_origin.y * sys + m_position.x;
            float ty = m_origin.x * sxs - m_origin.y * syc + m_position.y;

            m_transform = Transform( sxc, sys, tx,
                                    -sxs, syc, ty,
                                     0.f, 0.f, 1.f);

            m_isTransformDirty = false;
        }

        return m_transform;
	}

	void Transformable::rotate(float angle)
	{
		m_rotation += angle;
		m_rotation = fmod(360.f + fmod(m_rotation, 360.f), 360.f);
		m_isTransformDirty = true;
	}

} // namespace EmuCommon
