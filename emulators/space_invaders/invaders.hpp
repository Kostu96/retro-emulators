#pragma once
#include "cpu8080.hpp"
#include "disasm8080.hpp"

class Invaders
{
public:
    //const std::vector<DisassemblyLine>& getDisassembly() const { return m_disassembly; }

    //void handleKey(int key, int action) override;

    void loadROM(const char* filename) {}
    void reset();
    void update();

    Invaders();
private:
    static constexpr size_t VRAM_SIZE = 0x1C00;

    u8 memoryRead(u16 address);
    void memoryWrite(u16 address, u8 data);

    CPU8080 m_cpu;
    u8 m_ROM[0x2000];
    u8 m_RAM[0x2000];
    u8* m_VRAM = m_RAM + 0x400;
    //std::vector<DisassemblyLine> m_disassembly;
};
