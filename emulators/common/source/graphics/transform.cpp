#include "emu_common/graphics/transform.hpp"

namespace EmuCommon {

    Transform& Transform::translate(Vec2f offset)
    {
        Transform translation{ 1.f, 0.f, offset.x,
                               0.f, 1.f, offset.y,
                               0.f, 0.f, 1.f };

        return *this *= translation;
    }

    Vec2f Transform::transformVec2(Vec2f vec2) const
    {
        return { m_matrix[0] * vec2.x + m_matrix[4] * vec2.y + m_matrix[12],
                 m_matrix[1] * vec2.x + m_matrix[5] * vec2.y + m_matrix[13] };
    }

    FRect Transform::tranformRect(FRect rect) const
    {
        // Transform the 4 corners of the rectangle
        const Vec2f points[] = { transformVec2({rect.x,  rect.y }),
                                 transformVec2({rect.x,  rect.y + rect.height }),
                                 transformVec2({rect.x + rect.width, rect.y }),
                                 transformVec2({rect.x + rect.width, rect.y + rect.height }) };

        // Compute the bounding rectangle of the transformed points
        float left = points[0].x;
        float top = points[0].y;
        float right = points[0].x;
        float bottom = points[0].y;

        for (int i = 1; i < 4; ++i)
        {
            if      (points[i].x < left)   left = points[i].x;
            else if (points[i].x > right)  right = points[i].x;

            if      (points[i].y < top)    top = points[i].y;
            else if (points[i].y > bottom) bottom = points[i].y;
        }

        return { left, top, right - left, bottom - top };
    }

    Transform& operator*=(Transform& left, const Transform& right)
    {
        const float* a = left.m_matrix;
        const float* b = right.m_matrix;

        left = Transform(a[0] * b[0]  + a[4] * b[1]  + a[12] * b[3],
                         a[0] * b[4]  + a[4] * b[5]  + a[12] * b[7],
                         a[0] * b[12] + a[4] * b[13] + a[12] * b[15],
                         a[1] * b[0]  + a[5] * b[1]  + a[13] * b[3],
                         a[1] * b[4]  + a[5] * b[5]  + a[13] * b[7],
                         a[1] * b[12] + a[5] * b[13] + a[13] * b[15],
                         a[3] * b[0]  + a[7] * b[1]  + a[15] * b[3],
                         a[3] * b[4]  + a[7] * b[5]  + a[15] * b[7],
                         a[3] * b[12] + a[7] * b[13] + a[15] * b[15]);

        return left;
    }

} // namespace EmuCommon
