#pragma once
#include "../../emulator_core.hpp"
#include "cpu6502.hpp"
#include "io.hpp"

class PETCore :
    public EmulatorCore
{
public:
    PETCore();

    void getWindowSettings(WindowSettings& settings) override;
    std::span<u8> getMemory() override;
    void render(CharVertex* verts) override;
    void handleKey(int key, int action) override;

    void loadROM(const char* filename) override;
    void reset() override;
    void clock() override;
private:
    static void updateScreen() {}

    CPU6502 m_cpu;
    IO m_io;
};
