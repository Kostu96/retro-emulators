#pragma once
#include "../../shared/type_aliases.hpp"

#include <vector>

class CHIP8
{
public:
    /*const EmulatorSettings& getEmulatorSettings() const override { return m_emulatorSettings; }

    const size_t* getMemorySizes() const override { return m_memorySizes; }
    const int* getMemoryColsNumbers() const override { return m_memoryColsNumbers; }
    const std::vector<DisassemblyLine>& getDisassembly() const override { return m_disassembly; }
    const std::vector<StateEntry>& getState() const override { return m_state; }*/

    u8 getByteAt(u16 address, size_t /*memoryIndex*/) const { return m_memory[address]; }
    //u16 getPC() const override { return PC; }

    void handleKey(int key, int action);

    void loadROM(const char* filename);
    void reset();
    void update();

    /*void setClearCallback(ClearCallback callback) override { m_clear = callback; }
    void setRenderPointCallback(RenderPointCallback callback) override { m_renderPoint = callback; }*/

    CHIP8() = default;
private:
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

    std::vector<DisassemblyLine> m_disassembly;
};
