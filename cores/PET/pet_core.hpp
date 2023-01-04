#pragma once
#include "../../emulator_core.hpp"

class PETCore :
    public EmulatorCore
{
public:
    PETCore() = default;

    void getWindowSettings(WindowSettings& settings) override;
    std::span<u8> getMemory() override;
    void render(CharVertex* verts) override;
    void handleKey(int key, int action) override;

    void loadROM(const char* filename) override;
    void reset() override;
    void clock() override;
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
};
