#pragma once
#include "cpu8080.hpp"
#include "cartridge.hpp"
#include "ppu.hpp"
#include "timer.hpp"

class Gameboy
{
public:
    Gameboy();
    ~Gameboy();

    void update();

    void loadCartridge(const char* filename) { m_cartridge.loadFromFile(filename); }

    const glw::Texture* getScreenTexture() const { return m_PPU.getScreenTexture(); }

    // debug:
    const glw::Texture* getTileDataTexture() const { return m_PPU.getTileDataTexture(); }
private:
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
    u8 m_unmapBootloader;
    u8 m_HRAM[0x7F];
    u8 m_interruptEnables;
    u8 m_bootloader[256];
};
