#include "emu_common/graphics/rect_shape.hpp"

#include <SDL.h>

namespace EmuCommon {

    void RectShape::render(SDL_Renderer* renderer, Transform transform)
    {
        transform *= getTransform();
        FRect rect = transform.tranformRect({ 0.f, 0.f, m_size.x, m_size.y });

        SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
        SDL_RenderFillRectF(renderer, reinterpret_cast<SDL_FRect*>(&rect));
    }

} // namespace EmuCommon
