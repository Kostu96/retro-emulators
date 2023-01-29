#pragma once
#include "../../shared/emulator_core.hpp"
#include "../../shared/asm_common/asm_common.hpp"

class CHIP8Core :
    public EmulatorCore
{
public:
    const EmulatorSettings& getEmulatorSettings() const override { return m_emulatorSettings; }

    const size_t* getMemorySizes() const override { return m_memorySizes; }
    const int* getMemoryColsNumbers() const override { return m_memoryColsNumbers; }
    const std::vector<DisassemblyLine>& getDisassembly() const override { return m_disassembly; }
    const std::vector<StateEntry>& getState() const override { return m_state; }

    u8 getByteAt(u16 address, size_t memoryIndex) const { return m_memory[address]; }
    u16 getPC() const override { return PC; }

    void render(CharVertex* verts) const override;
    void handleKey(int key, int action) override;

    void loadROM(const char* filename) override;
    void reset() override;
    void update(double dt) override;

    void setRenderPointCallback(RenderPointCallback callback) override { m_renderPoint = callback; }

    CHIP8Core();
private:
    void updateState();

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
    u8* Stack = m_memory + 0xEA0;
    u8* GPR = m_memory + 0xEF0;
    u8* Screen = m_memory + 0xF00;
    u16 I : 12;
    u16 PC : 12;
    u8 SP : 4;
    u8 DT;
    u8 ST;
    bool keys[16];
    double m_elspsedTime;

    const EmulatorSettings m_emulatorSettings;
    std::vector<DisassemblyLine> m_disassembly;
    std::vector<StateEntry> m_state;

    RenderPointCallback m_renderPoint = nullptr;
};
