#pragma once
#include "emu_common/vec2.hpp"
#include "emu_common/graphics/rect.hpp"

namespace EmuCommon {
    
    class Transform
    {
    public:
        Transform() = default;

        Transform(float a00, float a01, float a02,
                  float a10, float a11, float a12,
                  float a20, float a21, float a22) :
            m_matrix{ a00, a10, 0.f, a20,
                      a01, a11, 0.f, a21,
                      0.f, 0.f, 1.f, 0.f,
                      a02, a12, 0.f, a22 } {}

        Transform& translate(Vec2f offset);

        Vec2f transformVec2(Vec2f vec2) const;
        FRect tranformRect(FRect rect) const;

        const float* getMatrix() const { return m_matrix; }

        friend Transform& operator*=(Transform& left, const Transform& right);
        friend Transform operator*(Transform left, const Transform& right) { return left *= right; }
    private:
        float m_matrix[16]{ 1.f, 0.f, 0.f, 0.f,
                            0.f, 1.f, 0.f, 0.f,
                            0.f, 0.f, 1.f, 0.f,
                            0.f, 0.f, 0.f, 1.f };
    };

} // namespace EmuCommon
