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

    void reset();
    void update();

    void loadCartridge(const char* filename);

    static u16 getScreenWidth() { return PPU::LCD_WIDTH; }
    static u16 getScreenHeight() { return PPU::LCD_HEIGHT; }
    std::span<u32> getScreenPixels() const { return m_PPU.getScreenPixels(); }

    // debug:
    static u16 getTileDataWidth() { return PPU::TILE_DATA_WIDTH; }
    static u16 getTileDataHeight() { return PPU::TILE_DATA_HEIGHT; }
    std::span<u32> getTileDataPixels() const { return m_PPU.getTileDataPixels(); }
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
    
    bool m_isRunning;
    bool m_hasCartridge;
};
