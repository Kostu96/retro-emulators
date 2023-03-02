#include "emu_common/graphics/rect_shape.hpp"

#include <SDL.h>

namespace EmuCommon {

    void RectShape::render(SDL_Renderer* renderer, const RenderStates& states)
    {
        SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
        const SDL_FRect rect{
            getPosition().x + states.position.x,
            getPosition().y + states.position.y,
            m_size.x * getScale().x * states.scale.x,
            m_size.y * getScale().y * states.scale.y
        };
        SDL_RenderFillRectF(renderer, &rect);
    }

} // namespace EmuCommon
