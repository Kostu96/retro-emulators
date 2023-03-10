#pragma once
#include "emu_common/graphics/transformable.hpp"
#include "emu_common/graphics/renderable.hpp"
#include "emu_common/graphics/rect_shape.hpp"
#include "emu_common/graphics/text.hpp"

class Logo :
    public EmuCommon::Transformable,
    public EmuCommon::Renderable
{
public:
    explicit Logo(const EmuCommon::SDLFont& font);

    void render(SDL_Renderer* renderer, EmuCommon::Transform transform = {}) override;

    const EmuCommon::Vec2f& getSize() const { return m_size; }
private:
    EmuCommon::Vec2f m_size;
    EmuCommon::RectShape m_background;
    EmuCommon::SDLText m_text1;
    EmuCommon::SDLText m_text2;
};
