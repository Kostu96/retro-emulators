#include "emu_common/graphics/rect_shape.hpp"

#include <SDL.h>

namespace EmuCommon {

    void RectShape::render(SDL_Renderer* renderer, Vec2f offset)
    {
        SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
        const SDL_FRect rect{
            getPosition().x + offset.x,
            getPosition().y + offset.y,
            m_size.x,
            m_size.y
        };
        SDL_RenderFillRectF(renderer, &rect);
    }

} // namespace EmuCommon
