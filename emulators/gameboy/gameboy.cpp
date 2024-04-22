#include "gameboy.hpp"
#include "gb_doctor.hpp"
#include "shared/source/address_range.hpp"

#include <cassert>
#include <iostream>
#include <iomanip>

static const AddressRange16 ROM_RANGE{       0x0000, 0x7FFF };
static const AddressRange16 VRAM_RANGE{      0x8000, 0x9FFF };
static const AddressRange16 EXTRAM_RANGE{    0xA000, 0xBFFF };
static const AddressRange16 WRAM_RANGE{      0xC000, 0xDFFF };
static const AddressRange16 OAM_RANGE{       0xFE00, 0xFE9F };
static const AddressRange16 UNUSED1_RANGE{   0xFEA0, 0xFEFF };
static const AddressRange16 SERIAL_RANGE{    0xFF01, 0xFF02 };
static const AddressRange16 TIMER_RANGE{     0xFF04, 0xFF07 };
static const AddressRange16 APU_RANGE{       0xFF10, 0xFF26 };
static const AddressRange16 UNUSED2_RANGE{   0xFF27, 0xFF3F };
static const AddressRange16 PPU_RANGE{       0xFF40, 0xFF4B };
static const AddressRange16 UNUSED3_RANGE{   0xFF7F, 0xFF7F };
static const AddressRange16 HRAM_RANGE{      0xFF80, 0xFFFE };

Gameboy::Gameboy() :
    m_PPU{ m_interruptFlags },
    m_WRAM{ new u8[0x2000] },
    m_timer{ m_interruptFlags },
    m_hasCartridge{ false }
{
    m_CPU.mapReadMemoryCallback([this](u16 address) { return memoryRead(address); });
    m_CPU.mapWriteMemoryCallback([this](u16 address, u8 data) { memoryWrite(address, data); });

    m_PPU.mapReadExternalMemoryCallback([this](u16 address) { return memoryRead(address); });

    //std::ifstream file{ "DMG_ROM.bin", std::ios_base::binary };
    //assert(file.is_open() && "Cannot open bootloader file");
    //file.read((char*)m_bootloader, 256);
    //file.close();

    reset();
    initGBDoctor();
}

Gameboy::~Gameboy()
{
    shutdownGBDoctor();

    delete[] m_WRAM;
}

void Gameboy::reset()
{
    std::memset(m_WRAM, 0, 0x2000);
    m_PPU.clearVRAM();

    m_CPU.reset();
    m_CPU.setAF(0x01B0);
    m_CPU.setBC(0x0013);
    m_CPU.setDE(0x00D8);
    m_CPU.setHL(0x014D);
    m_CPU.setSP(0xFFFE);
    m_CPU.setPC(0x0100);

    m_joypad = 0xCF;
    m_serialData = 0;
    m_serialControl = 0x7E;
    m_timer.reset();
    m_interruptFlags = 0xE1;
    m_APU.reset();
    m_PPU.reset();
    m_unmapBootloader = 0xFF;
    m_interruptEnables = 0xE0;

    m_serialBuffer[0] = '\0';
    m_serialBufferSize = 0;

    m_isRunning = true;
}

void Gameboy::update()
{
    if (m_hasCartridge && m_isRunning) {
        updateGBDoctor(m_unmapBootloader & 1, m_CPU.getCyclesLeft() == 1, m_CPU.getState(), [this](u16 address) { return memoryRead(address); });
        m_timer.clock();
        m_PPU.clock();
        m_CPU.clock();
        if (!m_CPU.isHandlingInterrupt()) {
            if ((m_interruptEnables & 1) & (m_interruptFlags & 1)) { // V-Blank
                if (m_CPU.interrupt(8))
                    m_interruptFlags &= ~0x1;
            }
            else if ((m_interruptEnables & 2) & (m_interruptFlags & 2)) { // LCD STAT
                if (m_CPU.interrupt(9))
                    m_interruptFlags &= ~0x2;
            }
            else if ((m_interruptEnables & 4) & (m_interruptFlags & 4)) { // Timer
                if (m_CPU.interrupt(10))
                    m_interruptFlags &= ~0x4;
            }
            else if ((m_interruptEnables & 8) & (m_interruptFlags & 8)) { // Serial
                if (m_CPU.interrupt(11))
                    m_interruptFlags &= ~0x8;
            }
            else if ((m_interruptEnables & 0x10) & (m_interruptFlags & 0x10)) { // Joypad
                if (m_CPU.interrupt(12))
                    m_interruptFlags &= ~0x10;
            }
        }
    }
}

void Gameboy::loadCartridge(const char* filename, bool quiet)
{
    m_isRunning = false;
    m_cartridge.loadFromFile(filename, quiet);
    m_hasCartridge = true;
}

void Gameboy::runUntilEndlessLoop()
{
    u16 lastPC = m_CPU.getState().PC;
    u8 counter = 0;
    while (counter < 10)
    {
        update();

        if (lastPC == m_CPU.getState().PC && !m_CPU.getState().IsHalted)
            counter++;
        else
            counter = 0;

        lastPC = m_CPU.getState().PC;
    }
}

void Gameboy::runUntilDebugBreak()
{
    u8 nextInstr = memoryRead(m_CPU.getState().PC);
    while (nextInstr != 0x40)
    {
        update();
        nextInstr = memoryRead(m_CPU.getState().PC);
    }
}

u8 Gameboy::memoryRead(u16 address)
{
    u16 offset;
    if (ROM_RANGE.contains(address, offset)) {
        if ((m_unmapBootloader & 1) == 0 && offset < 0x100) return m_bootloader[offset];
        return m_cartridge.load8(offset);
    }

    if (VRAM_RANGE.contains(address, offset)) return m_PPU.loadVRAM8(offset);
    if (EXTRAM_RANGE.contains(address, offset)) return m_cartridge.load8ExtRAM(offset);
    if (WRAM_RANGE.contains(address, offset)) return m_WRAM[offset];
    if (OAM_RANGE.contains(address, offset)) return m_PPU.loadOAM8(offset);
    if (address == 0xFF00) return m_joypad;
    if (SERIAL_RANGE.contains(address, offset)) {
        if (offset == 0) return m_serialData;
        if (offset == 1) return m_serialControl;
    }
    if (TIMER_RANGE.contains(address, offset)) return m_timer.load8(offset);
    if (APU_RANGE.contains(address, offset)) return m_APU.load8(offset);
    if (PPU_RANGE.contains(address, offset)) return m_PPU.load8(offset);
    if (address == 0xFF0F) return m_interruptFlags;
    if (address == 0xFF50) return m_unmapBootloader;
    if (HRAM_RANGE.contains(address, offset)) return m_HRAM[offset];
    if (address == 0xFFFF) return m_interruptEnables;

    std::cerr << "Unexpected memory read - " << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << address << '\n';
    return 0xFF;
}

void Gameboy::memoryWrite(u16 address, u8 data)
{
    u16 offset;
    if (ROM_RANGE.contains(address, offset)) { m_cartridge.store8(offset, data); return; }
    if (VRAM_RANGE.contains(address, offset)) { m_PPU.storeVRAM8(offset, data); return; }
    if (EXTRAM_RANGE.contains(address, offset)) { m_cartridge.store8ExtRAM(offset, data); return; }
    if (WRAM_RANGE.contains(address, offset)) { m_WRAM[offset] = data; return; }
    if (OAM_RANGE.contains(address, offset)) { m_PPU.storeOAM8(offset, data); return; }
    if (UNUSED1_RANGE.contains(address, offset)) { return; } // Ignore writes to unused memory

    if (address == 0xFF00) {
        m_joypad &= ~0x30;
        m_joypad |= data & 0x30;
        return;
    }

    if (SERIAL_RANGE.contains(address, offset)) {
        if (offset == 0) m_serialData = data;
        else if (offset == 1) {
            m_serialControl &= 0x7E;
            m_serialControl |= data & 0x81;
        }

        if (m_serialControl == 0xFF) {
            m_serialBuffer[m_serialBufferSize++] = m_serialData;
            m_serialBuffer[m_serialBufferSize] = '\0';
            m_serialBufferSize %= 64;
            m_serialControl &= 0x7F;
        }
        return;
    }

    if (TIMER_RANGE.contains(address, offset)) { m_timer.store8(offset, data); return; }
    if (APU_RANGE.contains(address, offset)) { return; } // TODO: sound
    if (UNUSED2_RANGE.contains(address, offset)) { return; } // Ignore writes to unused memory
    if (PPU_RANGE.contains(address, offset)) { m_PPU.store8(offset, data); return; }
    if (address == 0xFF0F) {
        m_interruptFlags &= 0xE0;
        m_interruptFlags |= data & 0x1F;
        return;
    }
    if (address == 0xFF50 && (m_unmapBootloader & 1) == 0) { m_unmapBootloader |= data & 1; return; }
    if (UNUSED3_RANGE.contains(address, offset)) { return; } // Ignore writes to unused memory
    if (HRAM_RANGE.contains(address, offset)) { m_HRAM[offset] = data; return; }
    if (address == 0xFFFF) { m_interruptEnables = data; return; }

    if (address >= 0xFF00) {
        std::cerr << "Unexpected IO region write - " << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << address;
        std::cerr << ':' << std::hex << std::setw(2) << (u16)data << '\n';
        return;
    }

    assert(false && "Unhandled memory write.");
}
