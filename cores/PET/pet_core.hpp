#pragma once
#include "../../shared/emulator_core.hpp"
#include "cpu6502.hpp"
#include "io.hpp"

class PETCore :
    public EmulatorCore
{
public:
    const EmulatorSettings& getEmulatorSettings() const override { return m_emulatorSettings; }

    const size_t* getMemorySizes() const override { return m_memorySizes; }
    const int* getMemoryColsNumbers() const override { return m_memoryColsNumbers; }
    const std::vector<DisassemblyLine>& getDisassembly() const override { return m_disassembly; }
    const std::vector<StateEntry>& getState() const override { return m_state; }

    u8 getByteAt(u16 address, size_t memoryIndex) const override { return m_cpu.load8(address); }
    u16 getPC() const override { return m_cpu.getPC(); }

    void render(CharVertex* verts) const override;
    void handleKey(int key, int action) override;

    void loadROM(const char* filename) override;
    void reset() override;
    void update(double dt) override;

    PETCore();
private:
    const size_t m_memorySizes[1] = { 0x10000 };
    const int m_memoryColsNumbers[1] = { 16 };

    CPU6502 m_cpu;
    IO m_io;

    const EmulatorSettings m_emulatorSettings;
    std::vector<DisassemblyLine> m_disassembly;
    std::vector<StateEntry> m_state;
};
