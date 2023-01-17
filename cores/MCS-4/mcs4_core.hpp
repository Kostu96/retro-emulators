#pragma once
#include "../../shared/emulator_core.hpp"
#include "cpu4040.hpp"

class MCS4Core :
    public EmulatorCore
{
public:
    const WindowSettings& getWindowSettings() const override { return m_windowSettings; }

    size_t getNumMemories() const override { return 2; }
    const size_t* getMemorySizes() const override { return m_memorySizes; }
    const std::vector<DisassemblyLine>& getDisassembly() const override { return m_disassembly; }
    const std::vector<StateEntry>& getState() const override { return m_state; }

    u8 getByteAt(u16 address, size_t memoryIndex) const override;
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
    void writeROMIO(u8 chip, u8 data);
    void writeRAMOut(u8 chip, u8 data);

    const size_t m_memorySizes[2] = { 0x100, 32 * 4 };

    CPU4040 m_cpu;

    const WindowSettings m_windowSettings;
    std::vector<DisassemblyLine> m_disassembly;
    std::vector<StateEntry> m_state;
};
