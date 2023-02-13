#pragma once
#include "cpu8080.hpp"
#include "apu.hpp"
#include "cartridge.hpp"
#include "ppu.hpp"
#include "timer.hpp"

#if defined(GAMEBOY_TESTS)
#define PRIVATE public
#else
#define PRIVATE private
#endif

class Gameboy
{
public:
    Gameboy();
    ~Gameboy();

    void reset();
    void update();

    void loadCartridge(const char* filename);
    const PPU& getPPU() const { return m_PPU; }

    const char* getSerialBuffer() const { return m_serialBuffer; }

    // test only:
    void runUntilDebugBreak();
PRIVATE:
    u8 memoryRead(u16 address);
    void memoryWrite(u16 address, u8 data);

    CPU8080 m_CPU;
    PPU m_PPU;
    Cartridge m_cartridge;
    u8* m_WRAM;
    u8 m_joypad;
    u8 m_serial[2];
    Timer m_timer;
    u8 m_interruptFlags;
    APU m_APU;
    u8 m_unmapBootloader;
    u8 m_HRAM[0x7F];
    u8 m_interruptEnables;
    u8 m_bootloader[256];
    
    bool m_isRunning;
    bool m_hasCartridge;

    char m_serialBuffer[65];
    u8 m_serialBufferSize;
};

#undef PRIVATE
