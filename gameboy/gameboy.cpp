#include "gameboy.hpp"

#include "../shared/memory_map.hpp"

#include <cassert>
#include <fstream>
#include <iostream>

static const AddressRange CART_RANGE{ 0x0000, 0x7FFF };
static const AddressRange VRAM_RANGE{ 0x8000, 0x9FFF };
static const AddressRange WRAM_RANGE{ 0xC000, 0xDFFF };
static const AddressRange OAM_RANGE{ 0xFE00, 0xFE9F };
static const AddressRange SERIAL_RANGE{ 0xFF01, 0xFF02 };
static const AddressRange TIMER_RANGE{ 0xFF04, 0xFF07 };
static const AddressRange APU_RANGE{ 0xFF10, 0xFF26 };
static const AddressRange PPU_RANGE{ 0xFF40, 0xFF4B };
static const AddressRange HRAM_RANGE{ 0xFF80, 0xFFFE };

Gameboy::Gameboy() :
    m_CPU{ CPU8080::Mode::GameBoy },
    m_WRAM{ new u8[0x2000] },
    m_joypad{ 0xFF },
    m_timer{ m_interruptFlags },
    m_interruptFlags{ 0 },
    m_unmapBootloader{ 0 },
    m_interruptEnables{ 0 }
{
    m_CPU.mapReadMemoryCallback([this](u16 address) { return memoryRead(address); });
    m_CPU.mapWriteMemoryCallback([this](u16 address, u8 data) { memoryWrite(address, data); });

    std::ifstream file{ "DMG_ROM.bin", std::ios_base::binary };
    assert(file.is_open() && "Cannot open bootloader file");
    file.read((char*)m_bootloader, 256);
    file.close();
    m_bootloader[0x0003] = 0x18;
    m_bootloader[0x0004] = 0x07; // Insert jump over VRAM clear routine

    m_CPU.reset();
    m_PPU.reset();
    m_timer.reset();
}

Gameboy::~Gameboy()
{
    delete[] m_WRAM;
}

void Gameboy::update()
{
    // bootloader routine to clear the VRAM
    if (m_unmapBootloader == 0 && m_CPU.getPC() == 0x0003) m_PPU.clearVRAM();

    m_CPU.clock();
    m_PPU.clock();
    m_timer.clock();
}

u8 Gameboy::memoryRead(u16 address)
{
    u16 offset;
    if (CART_RANGE.contains(address, offset)) {
        if (m_unmapBootloader == 0 && offset < 0x100) return m_bootloader[offset];
        return m_cartridge.load8(offset);
    }

    if (WRAM_RANGE.contains(address, offset)) return m_WRAM[offset];
    if (address == 0xFF00) return m_joypad;
    if (TIMER_RANGE.contains(address, offset)) return m_timer.load8(offset);
    if (APU_RANGE.contains(address, offset)) return u8{}; // TODO: sound
    if (PPU_RANGE.contains(address, offset)) return m_PPU.load8(offset);
    if (address == 0xFF0F) return m_interruptFlags;
    if (address == 0xFF50) return m_unmapBootloader;
    if (HRAM_RANGE.contains(address, offset)) return m_HRAM[offset];
    if (address == 0xFFFF) return m_interruptEnables;

    assert(false && "Unhandled memory read.");
    return u8{};
}

void Gameboy::memoryWrite(u16 address, u8 data)
{
    u16 offset;
    if (CART_RANGE.contains(address, offset)) { m_cartridge.store8(offset, data); return; }
    if (VRAM_RANGE.contains(address, offset)) { m_PPU.storeVRAM8(offset, data); return; }
    if (WRAM_RANGE.contains(address, offset)) { m_WRAM[offset] = data; return; }
    if (OAM_RANGE.contains(address, offset)) { m_PPU.storeOAM8(offset, data); return; }

    if (address == 0xFF00) {
        m_joypad &= ~0x30;
        m_joypad |= data & 0x30;
        return;
    }

    if (SERIAL_RANGE.contains(address, offset)) {
        m_serial[offset] = data;
        if (offset == 1 && data == 0x81) {
            std::cout << m_serial[0];
            m_serial[1] = 0;
        }
        return;
    }

    if (TIMER_RANGE.contains(address, offset)) { m_timer.store8(offset, data); return; }
    if (APU_RANGE.contains(address, offset)) { return; } // TODO: sound
    if (PPU_RANGE.contains(address, offset)) { m_PPU.store8(offset, data); return; }
    if (address == 0xFF0F) { m_interruptFlags = data & 0x1F; return; }
    if (address == 0xFF50 && m_unmapBootloader == 0) { m_unmapBootloader = data; return; }
    if (HRAM_RANGE.contains(address, offset)) { m_HRAM[offset] = data; return; }
    if (address == 0xFFFF) { m_interruptEnables = data & 0x1F; return; }

    assert(false && "Unhandled memory write.");
}
