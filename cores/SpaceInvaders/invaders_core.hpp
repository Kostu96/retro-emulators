#pragma once
#include "../../shared/emulator_core.hpp"
#include "cpu8080.hpp"
#include "disasm8080.hpp"

class InvadersCore :
    public EmulatorCore
{
public:
    const WindowSettings& getWindowSettings() const override { return m_windowSettings; }

    const std::vector<DisassemblyLine>& getDisassembly() const override { return m_disassembly; }
    const std::vector<StateEntry>& getState() const override { return m_state; }
    
    u8 getByteAt(u16 address) const override;
    u16 getPC() const override { return m_cpu.getPC(); }

    void render(CharVertex* verts) override;
    void handleKey(int key, int action) override;

    void loadROM(const char* filename) override {}
    void reset() override;
    void clock() override;
    
    InvadersCore();
private:
    CPU8080 m_cpu;

    const WindowSettings& m_windowSettings;
    std::vector<DisassemblyLine> m_disassembly;
    std::vector<StateEntry> m_state;
};
