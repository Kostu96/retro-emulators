#pragma once
#include "../../shared/emulator_core.hpp"
#include "cpu6502.hpp"
#include "io.hpp"

class PETCore :
    public EmulatorCore
{
public:
    const WindowSettings& getWindowSettings() const override { return m_windowSettings; }

    const size_t* getMemorySizes() const override { return m_memorySizes; }
    const int* getMemoryColsNumbers() const override { return 0; }
    const std::vector<DisassemblyLine>& getDisassembly() const override { return m_disassembly; }
    const std::vector<StateEntry>& getState() const override { return m_state; }

    u8 getByteAt(u16 address, size_t memoryIndex) const override { return 0xCD; } // TODO: temp
    u16 getPC() const override { return m_cpu.getPC(); }

    void render(CharVertex* verts) override;
    void handleKey(int key, int action) override;

    void loadROM(const char* filename) override;
    void reset() override;
    void clock() override;

    PETCore();
private:
    const size_t m_memorySizes[1] = { 0 };

    CPU6502 m_cpu;
    IO m_io;

    const WindowSettings m_windowSettings;
    std::vector<DisassemblyLine> m_disassembly;
    std::vector<StateEntry> m_state;
};
