#include "psx.hpp"

#include "shared/source/address_range.hpp"
#include "shared/source/file_io.hpp"

#include <cassert>
#include <iostream>
#include <iomanip>

#define HEX(value, w) "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(w) << (u32)value

namespace PSX {

    static constexpr u32 REGION_MASK[] = {
        // KUSEG 2048MB
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        // KSEG0 512MB
        0x7FFFFFFF,
        // KSEG1 512MB
        0x1FFFFFFF,
        // KSEG2 1024MB
        0xFFFFFFFF, 0xFFFFFFFF
    };

    static u32 maskRegion(u32 address)
    {
        u32 index = address >> 29;
        return address & REGION_MASK[index];
    }

    // TODO: address decoding
    static constexpr AddressRange32 RAM_RANGE{ 0x00000000, 0x00000000 + RAM_SIZE - 1 };

    static constexpr AddressRange32 EXPANSION1_RANGE{ 0x1F000000, 0x1F000000 + 8 * 1024 * 1024 - 1 };

    static constexpr AddressRange32 MEM_CTRL_RANGE{ 0x1F801000, 0x1F801023 };

    static constexpr AddressRange32 RAM_SIZE_RANGE{ 0x1F801060, 0x1F801063 };

    static constexpr AddressRange32 IRQ_CTRL_RANGE{ 0x1F801070, 0x1F801077 };

    static constexpr AddressRange32 SPU_RANGE{ 0x1F801C00, 0x1F801E7F };

    static constexpr AddressRange32 EXPANSION2_RANGE{ 0x1F802000, 0x1F802080 };

    static constexpr AddressRange32 BIOS_RANGE{ 0x1FC00000, 0x1FC00000 + BIOS_SIZE - 1 };

    static constexpr AddressRange32 CACHE_CTRL_RANGE{ 0xFFFE0130, 0xFFFE0133 };

    void Emulator::reset()
    {
        m_CPU.reset();
    }

    void Emulator::clock()
    {
        DisassemblyLine line;
        m_CPU.clock(line);
        auto i = m_disasm.begin();
        for (; i != m_disasm.end(); i++)
        {
            if (i->address >= line.address)
                break;
        }
        if (m_disasm.empty() || i == m_disasm.end())
        {
            m_disasm.push_back(line);
            return;
        }
        if (line.address == i->address) *i = line;
        else m_disasm.insert(i, line);
    }

    Emulator::Emulator(Disassembly& disasm) :
        m_disasm{ disasm }
    {
        size_t size = BIOS_SIZE;
        if (!readFile("rom/psx/SCPH-1001.bin", (char*)m_BIOS, size, true))
            std::cerr << "Could not read BIOS ROM file!\n";

        m_CPU.mapRead8MemoryCallback([this](u32 address) { return memoryRead8(address); });
        m_CPU.mapRead32MemoryCallback([this](u32 address) { return memoryRead32(address); });
        m_CPU.mapWrite8MemoryCallback([this](u32 address, u8 data) { memoryWrite8(address, data); });
        m_CPU.mapWrite16MemoryCallback([this](u32 address, u16 data) { memoryWrite16(address, data); });
        m_CPU.mapWrite32MemoryCallback([this](u32 address, u32 data) { memoryWrite32(address, data); });

        m_CPU.reset();
    }

    u8 Emulator::memoryRead8(u32 address) const
    {
        address = maskRegion(address);

        u32 offset;
        if (RAM_RANGE.contains(address, offset)) return m_RAM[offset];
        
        if (EXPANSION1_RANGE.contains(address, offset)) return 0xFF; // Full ones for no expansion implemented

        if (BIOS_RANGE.contains(address, offset))  return m_BIOS[offset];

        std::cerr << "Unhandled read8 from memory at address: " << HEX(address, 8) << '\n';
        assert(false);
        return 0;
    }

    u32 Emulator::memoryRead32(u32 address) const
    {
        assert(address % 4 == 0 && "Unaligned memory access!");

        address = maskRegion(address);

        u32 offset;
        if (RAM_RANGE.contains(address, offset)) {
            u32 b0 = m_RAM[offset];
            u32 b1 = m_RAM[offset + 1];
            u32 b2 = m_RAM[offset + 2];
            u32 b3 = m_RAM[offset + 3];
            return (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
        }

        if (BIOS_RANGE.contains(address, offset)) {
            u32 b0 = m_BIOS[offset];
            u32 b1 = m_BIOS[offset + 1];
            u32 b2 = m_BIOS[offset + 2];
            u32 b3 = m_BIOS[offset + 3];
            return (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
        }

        std::cerr << "Unhandled read32 from memory at address: " << HEX(address, 8) << '\n';
        assert(false);
        return 0;
    }

    void Emulator::memoryWrite8(u32 address, u8 data)
    {
        address = maskRegion(address);

        u32 offset;
        if (RAM_RANGE.contains(address, offset)) {
            m_RAM[offset] = data;
            return;
        }

        if (EXPANSION2_RANGE.contains(address, offset)) {
            std::cerr << "Unhandled 8 bit write to: " << HEX(address, 8) <<
                " EXPANSION2(" << HEX(offset, 2) << "): " << HEX(data, 2) << '\n';
            return;
        }

        std::cerr << "Unhandled write8 to memory at address: " << HEX(address, 8) << " data: " << HEX(data, 2) << '\n';
        assert(false);
    }

    void Emulator::memoryWrite16(u32 address, u16 data)
    {
        assert(address % 2 == 0 && "Unaligned memory access!");

        address = maskRegion(address);

        u32 offset;
        if (SPU_RANGE.contains(address, offset)) {
            std::cerr << "Unhandled 16 bit write to: " << HEX(address, 8) <<
                " SPU(" << HEX(offset, 3) << "): " << HEX(data, 4) << '\n';
            return;
        }

        std::cerr << "Unhandled write16 to memory at address: " << HEX(address, 8) << " data: " << HEX(data, 4) << '\n';
        assert(false);
    }

    void Emulator::memoryWrite32(u32 address, u32 data)
    {
        assert(address % 4 == 0 && "Unaligned memory access!");

        address = maskRegion(address);

        u32 offset;
        if (MEM_CTRL_RANGE.contains(address, offset)) {
            switch (offset) {
            case 0:
                if (data != 0x1F000000) {
                    std::cerr << "Bad write to expansion 1 base address!\n";
                    assert(false);
                }
                break;
            case 4:
                if (data != 0x1F802000) {
                    std::cerr << "Bad write to expansion 2 base address!\n";
                    assert(false);
                }
                break;
            default:
                std::cerr << "Unhandled 32 bit write to: " << HEX(address, 8) <<
                    " MEM_CTRL(" << HEX(offset, 2) << "): " << HEX(data, 8) << '\n';
            }
            return;
        }

        if (RAM_SIZE_RANGE.contains(address, offset)) {
            std::cerr << "Unhandled 32 bit write to: " << HEX(address, 8) <<
                " RAM_SIZE(" << HEX(offset, 2) << "): " << HEX(data, 8) << '\n';
            return;
        }

        if (IRQ_CTRL_RANGE.contains(address, offset)) {
            std::cerr << "Unhandled 32 bit write to: " << HEX(address, 8) <<
                " IRQ_CTRL(" << HEX(offset, 2) << "): " << HEX(data, 8) << '\n';
            return;
        }

        if (RAM_RANGE.contains(address, offset)) {
            m_RAM[offset] = data & 0xFF;
            m_RAM[offset + 1] = (data >> 8) & 0xFF;
            m_RAM[offset + 2] = (data >> 16) & 0xFF;
            m_RAM[offset + 3] = (data >> 24) & 0xFF;
            return;
        }

        if (CACHE_CTRL_RANGE.contains(address, offset)) {
            std::cerr << "Unhandled 32 bit write to: " << HEX(address, 8) <<
                " CACHE_CTRL(" << HEX(offset, 2) << "): " << HEX(data, 8) << '\n';
            return;
        }

        std::cerr << "Unhandled write32 to memory at address: " << HEX(address, 8) << " data: " << HEX(data, 8) << '\n';
        assert(false);
    }

} // namespace PSX
