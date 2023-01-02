#pragma once
#include "types.hpp"

class EmulatorCore
{
public:
    struct WindowSettings
    {
        u16 width;
        u16 height;
        char title[252];
    };

    virtual ~EmulatorCore() = default;

    virtual void getWindowSettings(WindowSettings& settings) = 0;
};
