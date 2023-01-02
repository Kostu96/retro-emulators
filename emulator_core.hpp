#pragma once

class EmulatorCore
{
public:
    virtual ~EmulatorCore() = default;

    virtual void fn() = 0;
};
