#pragma once
#include "../../shared/emulator_core.hpp"
#include "cpu8008.hpp"

class MCS8Core :
    public EmulatorCore
{
public:
    const EmulatorSettings& getEmulatorSettings() const override { return m_emulatorSettings; }

    const size_t* getMemorySizes() const override { return m_memorySizes; }
    const int* getMemoryColsNumbers() const override { return m_memoryColsNumbers; }
    const std::vector<DisassemblyLine>& getDisassembly() const override { return m_disassembly; }
    const std::vector<StateEntry>& getState() const override { return m_state; }

    u8 getByteAt(u16 address, size_t memoryIndex) const override;
    u16 getPC() const override { return 0; }

    void loadROM(const char* filename) override;
    void reset() override;
    void update(double dt) override;
    
    MCS8Core();
private:
    void updateState();

    const size_t m_memorySizes[1] = { 0xC00 };
    const int m_memoryColsNumbers[1] = { 8 };

    CPU8008 m_cpu;

    const EmulatorSettings m_emulatorSettings;
    std::vector<DisassemblyLine> m_disassembly;
    std::vector<StateEntry> m_state;
};
