#pragma once
#include "emu_common/graphics/sprite.hpp"
#include "emu_common/gui/button.hpp"

#include <ccl/non_copyable.h>

class TwowayButton :
    public EmuCommon::Transformable,
    public EmuCommon::Renderable,
    public ccl::NonCopyable
{
public:
    TwowayButton();
    ~TwowayButton();
private:

};
