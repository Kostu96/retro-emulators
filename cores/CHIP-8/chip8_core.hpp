#pragma once
#include "../../shared/emulator_core.hpp"

class CHIP8Core :
    public EmulatorCore
{
public:
    const WindowSettings& getWindowSettings() const override { return m_windowSettings; }

    void render(CharVertex* verts) override;
    void handleKey(int key, int action) override;

    void loadROM(const char* filename) override;
    void reset() override;
    void clock() override;

    CHIP8Core();
private:
    u8 Memory[0x800];
    u8* Stack = Memory + 0x6A0;
    u8* GPR = Memory + 0x6F0;
    u8* Screen = Memory + 0x700;
    u16 I;
    u16 PC : 12;
    u8 SP : 4;
    u8 DT;
    u8 ST;
    bool keys[16];

    const WindowSettings m_windowSettings;
};
