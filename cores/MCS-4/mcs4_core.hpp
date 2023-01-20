#pragma once
#include "../../shared/emulator_core.hpp"
#include "cpu4040.hpp"

class MCS4Core :
    public EmulatorCore
{
public:
    const EmulatorSettings& getEmulatorSettings() const override { return m_emulatorSettings; }

    size_t getNumMemories() const override { return 3; }
    const size_t* getMemorySizes() const override { return m_memorySizes; }
    const int* getMemoryColsNumbers() const override { return m_memoryColsNumbers; }
    const std::vector<DisassemblyLine>& getDisassembly() const override { return m_disassembly; }
    const std::vector<StateEntry>& getState() const override { return m_state; }

    u8 getByteAt(u16 address, size_t memoryIndex) const override;
    u16 getPC() const override { return m_cpu.getPC(); }

    void render(CharVertex* verts) override;
    void handleKey(int key, int action) override;

    void loadROM(const char* filename) override;
    void reset() override;
    void update(double dt) override;
    
    MCS4Core();
private:
    void updateState();

    u8 readROMIO(u8 chip);
    void writeROMIO(u8 chip, u8 data);
    void writeRAMOut(u8 chip, u8 data);

    const size_t m_memorySizes[3] = { 0x100, 32 * 4, 8 * 4 };
    const int m_memoryColsNumbers[3] = { 16, 8, 2 };

    CPU4040 m_cpu;

    const EmulatorSettings m_emulatorSettings;
    std::vector<DisassemblyLine> m_disassembly;
    std::vector<StateEntry> m_state;
};
