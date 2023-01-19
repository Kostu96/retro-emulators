#pragma once
#include "../../shared/emulator_core.hpp"

class CHIP8Core :
    public EmulatorCore
{
public:
    const WindowSettings& getWindowSettings() const override { return m_windowSettings; }

    const size_t* getMemorySizes() const override { return m_memorySizes; }
    const int* getMemoryColsNumbers() const override { return m_memoryColsNumbers; }
    const std::vector<DisassemblyLine>& getDisassembly() const override { return m_disassembly; }
    const std::vector<StateEntry>& getState() const override { return m_state; }

    u8 getByteAt(u16 address, size_t memoryIndex) const { return m_memory[address]; }
    u16 getPC() const override { return PC; }

    void render(CharVertex* verts) override;
    void handleKey(int key, int action) override;

    void loadROM(const char* filename) override;
    void reset() override;
    void clock() override;

    CHIP8Core();
private:
    void updateState();

    union Instruction
    {
        struct
        {
            u8 n1 : 4;
            u8 n2 : 4;
            u8 n3 : 4;
            u8 n4 : 4;
        } nibbles;
        u16 word;
    };

    static constexpr u16 CHIP8_WIDTH = 64;
    static constexpr u16 CHIP8_HEIGHT = 32;
    static constexpr u16 ZOOM = 8;
    static constexpr u16 WINDOW_WIDTH = CHIP8_WIDTH * ZOOM;
    static constexpr u16 WINDOW_HEIGHT = CHIP8_HEIGHT * ZOOM;

    static const size_t m_numMemories = 1;
    static const size_t m_memorySize = 0x1000;
    const size_t m_memorySizes[m_numMemories] = { m_memorySize };
    const int m_memoryColsNumbers[m_numMemories] = { 8 };

    u8 m_memory[m_memorySize];
    u8* Stack = m_memory + 0x6A0;
    u8* GPR = m_memory + 0x6F0;
    u8* Screen = m_memory + 0x700;
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
