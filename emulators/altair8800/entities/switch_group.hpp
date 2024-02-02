#pragma once
#include "emu_common/graphics/sprite.hpp"
#include "emu_common/graphics/text.hpp"
#include "emu_common/gui/checkbox.hpp"

class SwitchGroup :
    public EmuCommon::Transformable,
    public EmuCommon::Renderable
{
public:
    SwitchGroup(const EmuCommon::SDLFont& font,
                const EmuCommon::SDLTexture& texture,
                uint8_t count,
                const char** labels,
                float spacing,
                float tripletSpacing = 0,
                uint8_t originAtIndex = 0);

    ~SwitchGroup();

    void onEvent(SDL_Event& e);
    void handleMousePos(EmuCommon::Vec2i position);
    void render(SDL_Renderer* renderer, EmuCommon::Transform transform = {}) override;

    const EmuCommon::Vec2f& getSize() const { return m_size; }
    uint16_t getStates() const { return m_states; }

    SwitchGroup(const SwitchGroup&) = delete;
    SwitchGroup& operator=(const SwitchGroup&) = delete;
private:
    EmuCommon::SDLText* m_labels = nullptr;
    EmuCommon::GUI::ImageCheckbox* m_switches = nullptr;
    EmuCommon::Vec2f m_size;
    uint16_t m_states = 0;
    uint8_t m_count;
};
