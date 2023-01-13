#pragma once
#include "../../shared/emulator_core.hpp"
#include "cpu4040.hpp"

class MCS4Core :
    public EmulatorCore
{
public:
    const WindowSettings& getWindowSettings() const override { return m_windowSettings; }

    void render(CharVertex* verts) override;
    void handleKey(int key, int action) override;

    void loadROM(const char* filename) override;
    void reset() override;
    void clock() override;
    
    MCS4Core();
private:
    CPU4040 m_cpu;

    const WindowSettings m_windowSettings;
};
