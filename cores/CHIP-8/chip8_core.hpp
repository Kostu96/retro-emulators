#pragma once
#include "../../shared/emulator_core.hpp"

class CHIP8Core :
    public EmulatorCore
{
public:
    const WindowSettings& getWindowSettings() const override { return m_windowSettings; }

    const size_t* getMemorySizes() const override { return 0; }
    const int* getMemoryColsNumbers() const override { return 0; }
    const std::vector<DisassemblyLine>& getDisassembly() const override { return m_disassembly; }
    const std::vector<StateEntry>& getState() const override { return m_state; }

    u8 getByteAt(u16 address, size_t memoryIndex) const { return 0xCD; } // TODO: temp
    u16 getPC() const override { return PC; }

    void render(CharVertex* verts) override;
    void handleKey(int key, int action) override;

    void loadROM(const char* filename) override;
    void reset() override;
    void clock() override;

    CHIP8Core();
private:
    static const size_t m_numMemories = 1;
    static const size_t m_memorySize = 0x800;
    const size_t m_memorySizes[m_numMemories] = { m_memorySize };

    u8 Memory[m_memorySize];
    u8* Stack = Memory + 0x6A0;
    u8* GPR = Memory + 0x6F0;
    u8* Screen = Memory + 0x700;
    u16 I;
    u16 PC : 12;
    u8 SP : 4;
    u8 DT;
    u8 ST;
    bool keys[16];

    const WindowSettings m_windowSettings;
    std::vector<DisassemblyLine> m_disassembly;
    std::vector<StateEntry> m_state;
};
