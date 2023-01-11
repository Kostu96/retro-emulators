#pragma once
#include "../../emulator_core.hpp"
#include "cpu8080.hpp"

class InvadersCore :
    public EmulatorCore
{
public:
    InvadersCore();

    void getWindowSettings(WindowSettings& settings) override;
    u8 readByte(u16 address) override;
    void render(CharVertex* verts) override;
    void handleKey(int key, int action) override;

    void loadROM(const char* filename) override {}
    void reset() override;
    void clock() override;
private:
    CPU8080 m_cpu;
};
