#pragma once
#include "emu_common/graphics/transformable.hpp"
#include "emu_common/graphics/renderable.hpp"
#include "emu_common/graphics/color.hpp"

namespace EmuCommon {

    class RectShape :
        public Transformable,
        public Renderable
    {
    public:
        RectShape() = default;
        explicit RectShape(Vec2f size, Color color = {}) :
            m_size{ size },
            m_color{ color } {}

        void render(SDL_Renderer* renderer, const RenderStates& states = {}) override;

        void setSize(Vec2f size) { m_size = size; }
        Vec2f getSize() const { return m_size; }
    private:
        Vec2f m_size;
        Color m_color;
    };

} // namespace EmuCommon
