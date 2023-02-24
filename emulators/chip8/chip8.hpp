#pragma once
#include "../../shared/disassembly_line.hpp"

#include <ccl/types.hpp>

#include <span>
#include <vector>

class CHIP8
{
public:
    static constexpr u16 CHIP8_WIDTH = 64;
    static constexpr u16 CHIP8_HEIGHT = 32;

    CHIP8() { reset(); }

    void loadProgram(const char* filename);
    void reset();
    void update(double dt);

    void handleKey(int key, int action);
    std::span<const u32> getScreenPixels() const { return { m_screenPixels, CHIP8_WIDTH * CHIP8_HEIGHT }; }
    const std::vector<DisassemblyLine>& getDisassembly() const { return m_disassembly; }
private:
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
    u32 m_screenPixels[CHIP8_WIDTH * CHIP8_HEIGHT] ;
};
