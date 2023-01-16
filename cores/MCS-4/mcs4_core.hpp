#pragma once
#include "../../shared/emulator_core.hpp"
#include "cpu4040.hpp"

class MCS4Core :
    public EmulatorCore
{
public:
    const WindowSettings& getWindowSettings() const override { return m_windowSettings; }

    const std::vector<DisassemblyLine>& getDisassembly() const override { return m_disassembly; }
    const std::vector<StateEntry>& getState() const override { return m_state; }

    u16 getPC() const override { return m_cpu.getPC(); }

    void render(CharVertex* verts) override;
    void handleKey(int key, int action) override;

    void loadROM(const char* filename) override;
    void reset() override;
    void clock() override;
    
    MCS4Core();
private:
    void updateState();

    u8 readROMIO(u8 chip);
    void writeRAMOut(u8 chip, u8 data);

    CPU4040 m_cpu;

    const WindowSettings m_windowSettings;
    std::vector<DisassemblyLine> m_disassembly;
    std::vector<StateEntry> m_state;
};
