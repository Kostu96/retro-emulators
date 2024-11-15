#pragma once
#include "shared/source/devices/cpu8080/cpu8080.hpp"
#include "video.hpp"
#include "io.hpp"

class Invaders
{
public:
    void reset();
    void clock();
    void runUntilNextInstruction();

    const CPU8080& getCPU() const { return m_cpu; }
    const Video& getVideo() const { return m_video; }
    u8 memoryRead(u16 address) const;

    Invaders();
private:
    static constexpr size_t VRAM_SIZE = 0x1C00;

    void memoryWrite(u16 address, u8 data);

    CPU8080 m_cpu;
    u8 m_ROM[0x2000];
    u8 m_RAM[0x2000];
    u8* m_VRAM = m_RAM + 0x400;
    Video m_video;
    IO m_io;
};
