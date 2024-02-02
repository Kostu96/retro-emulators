#pragma once
#include "emu_common/graphics/sprite.hpp"
#include "emu_common/gui/button.hpp"

class TwowayButton :
    public EmuCommon::Transformable,
    public EmuCommon::Renderable
{
public:
    TwowayButton();
    ~TwowayButton();

    TwowayButton(const TwowayButton&) = delete;
    TwowayButton& operator=(const TwowayButton&) = delete;
private:

};
