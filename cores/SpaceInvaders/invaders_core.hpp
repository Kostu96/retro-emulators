#pragma once
#include "../../emulator_core.hpp"
#include "cpu8080.hpp"
#include "disasm8080.hpp"

class InvadersCore :
    public EmulatorCore
{
public:
    InvadersCore();

    void getWindowSettings(WindowSettings& settings) const override;
    u8 getByteAt(u16 address) const override;
    u16 getPC() const override { return m_cpu.getPC(); }
    const std::vector<DisassemblyLine>& getDisassembly() const override { return m_disassembly; }

    void render(CharVertex* verts) override;
    void handleKey(int key, int action) override;

    void loadROM(const char* filename) override {}
    void reset() override;
    void clock() override;
private:
    CPU8080 m_cpu;

    std::vector<DisassemblyLine> m_disassembly;
};
