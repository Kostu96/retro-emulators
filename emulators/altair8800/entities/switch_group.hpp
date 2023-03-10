#pragma once
#include "emu_common/graphics/transformable.hpp"
#include "emu_common/graphics/renderable.hpp"
#include "emu_common/graphics/sprite.hpp"
#include "emu_common/graphics/text.hpp"
#include "emu_common/gui/checkbox.hpp"

#include <ccl/non_copyable.h>
#include <ccl/types.hpp>

class SwitchGroup :
    public EmuCommon::Transformable,
    public EmuCommon::Renderable,
    public ccl::NonCopyable
{
public:
    SwitchGroup(const EmuCommon::SDLFont& font,
                const EmuCommon::SDLTexture& texture,
                u8 count,
                const char** labels,
                float spacing,
                float tripletSpacing = 0,
                u8 originAtIndex = 0);

    ~SwitchGroup();

    void onEvent(SDL_Event& e);
    void handleMousePos(EmuCommon::Vec2i position);
    void render(SDL_Renderer* renderer, EmuCommon::Transform transform = {}) override;

    const EmuCommon::Vec2f& getSize() const { return m_size; }
    u16 getStates() const { return m_states; }
private:
    EmuCommon::SDLText* m_labels = nullptr;
    EmuCommon::GUI::ImageCheckbox* m_switches = nullptr;
    EmuCommon::Vec2f m_size;
    u16 m_states = 0;
    u8 m_count;
};
