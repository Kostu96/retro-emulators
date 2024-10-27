#pragma once
#include "shared/source/devices/cpu8080/cpu8080.hpp"
#include "shared/source/devices/cpu8080/disasm8080.hpp"

class Invaders
{
public:
    void loadROM(const char* filename) {}
    void reset();
    void clock();

    const CPU8080& getCPU() const { return m_cpu; }

    Invaders();
private:
    static constexpr size_t VRAM_SIZE = 0x1C00;

    u8 memoryRead(u16 address) const;
    void memoryWrite(u16 address, u8 data);

    CPU8080 m_cpu;
    u8 m_ROM[0x2000];
    u8 m_RAM[0x2000];
    u8* m_VRAM = m_RAM + 0x400;
};
