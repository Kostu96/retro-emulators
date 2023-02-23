#pragma once
#include "../../shared/disassembly_line.hpp"

#include <ccl/types.hpp>

#include <vector>

class CHIP8
{
public:
    CHIP8() = default;

    u8 getByteAt(u16 address, size_t /*memoryIndex*/) const { return m_memory[address]; }
    //u16 getPC() const override { return PC; }

    void handleKey(int key, int action);

    void loadProgram(const char* filename);
    void reset();
    void update();

    const std::vector<DisassemblyLine>& getDisassembly() const { return m_disassembly; }
    /*void setClearCallback(ClearCallback callback) override { m_clear = callback; }
    void setRenderPointCallback(RenderPointCallback callback) override { m_renderPoint = callback; }*/
private:
    static constexpr u16 CHIP8_WIDTH = 64;
    static constexpr u16 CHIP8_HEIGHT = 32;

    static constexpr size_t MEMORY_SIZE = 0x1000;

    u8 m_memory[MEMORY_SIZE];
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
