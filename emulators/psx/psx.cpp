#include "psx.hpp"

#include "shared/source/address_range.hpp"
#include "shared/source/file_io.hpp"

#include <cassert>
#include <iostream>
#include <iomanip>

#define HEX(value, w) "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(w) << (u32)value

#define PRINT_UNHANDLED_WRITE(bits, label, offsetW, dataW) std::cerr << "Unhandled "#bits" bit write to: " << HEX(address, 8) << " "#label"(" << HEX(offset, offsetW) << "): " << HEX(data, dataW) << '\n'

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

    static constexpr AddressRange32 DMA_RANGE{    0x1F801080, 0x1F8010FF };
    static constexpr AddressRange32 TIMERS_RANGE{ 0x1F801100, 0x1F80112F };

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
        if (m_enableBIOSPatches) {
            auto it = m_BIOSPatches.find(m_CPU.getCPUStatus().PC);
            if (it != m_BIOSPatches.end())
                it->second();
        }

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
        if (!readFile("rom/psx/SCPH-1001.bin", (char*)m_BIOS, size, true)) {
            std::cerr << "Could not read BIOS ROM file!\n";
            assert(false);
        }

        m_BIOSPatches.emplace(0xBFC02B60, [this]() {
            auto& status = m_CPU.getCPUStatus();
            std::cout << "BIOS memcpy detected\n"
                << "  dst: " << std::hex << status.regs[4] << '\n'
                << "  src: " << std::hex << status.regs[5] << '\n'
                << "  len: " << std::hex << status.regs[6] << '\n';

            bool isMemcpyValid = true;
            u8* dstPtr = nullptr, * srcPtr = nullptr;
            u32 dstAddress = maskRegion(status.regs[4]);
            u32 dstOffset;
            if (RAM_RANGE.contains(dstAddress, dstOffset)) {
                dstPtr = m_RAM + dstOffset;
            }
            else {
                std::cout << "memcpy patch could not be applied to dst address!\n";
                isMemcpyValid = false;
            }

            u32 srcAddress = maskRegion(status.regs[5]);
            u32 srcOffset;
            if (BIOS_RANGE.contains(srcAddress, srcOffset)) {
                srcPtr = m_BIOS + srcOffset;
            }
            else {
                std::cout << "memcpy patch could not be applied to src address!\n";
                isMemcpyValid = false;
            }

            if (isMemcpyValid) {
                std::memcpy(dstPtr, srcPtr, status.regs[6]);
                m_CPU.overrideCPURegister(6, 0);
                std::cout << "BIOS memcpy patch successful!\n";
            }
        });

        m_CPU.mapRead8MemoryCallback([this](u32 address) { return memoryRead8(address); });
        m_CPU.mapRead16MemoryCallback([this](u32 address) { return memoryRead16(address); });
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

    u16 Emulator::memoryRead16(u32 address) const
    {
        address = maskRegion(address);

        u32 offset;
        if (RAM_RANGE.contains(address, offset)) {
            u16 b0 = m_RAM[offset + 0];
            u16 b1 = m_RAM[offset + 1];
            return (b1 << 8) | b0;
        }

        if (SPU_RANGE.contains(address, offset)) return 0; // TODO: temp

        std::cerr << "Unhandled read16 from memory at address: " << HEX(address, 8) << '\n';
        assert(false);
        return 0;
    }

    u32 Emulator::memoryRead32(u32 address) const
    {
        assert(address % 4 == 0 && "Unaligned memory access!");

        address = maskRegion(address);

        u32 offset;
        if (RAM_RANGE.contains(address, offset)) {
            u32 b0 = m_RAM[offset + 0];
            u32 b1 = m_RAM[offset + 1];
            u32 b2 = m_RAM[offset + 2];
            u32 b3 = m_RAM[offset + 3];
            return (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
        }

        if (IRQ_CTRL_RANGE.contains(address, offset))  return 0; // TODO: temp

        if (DMA_RANGE.contains(address, offset))  return 0; // TODO: temp

        if (BIOS_RANGE.contains(address, offset)) {
            u32 b0 = m_BIOS[offset + 0];
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
            PRINT_UNHANDLED_WRITE(8, EXPANSIO2, 2, 2);
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
        if (RAM_RANGE.contains(address, offset)) {
            m_RAM[offset + 0] = (data >> 0) & 0xFF;
            m_RAM[offset + 1] = (data >> 8) & 0xFF;
            return;
        }

        if (TIMERS_RANGE.contains(address, offset)) {
            PRINT_UNHANDLED_WRITE(16, TIMERS, 2, 4);
            return;
        }

        if (SPU_RANGE.contains(address, offset)) {
            //PRINT_UNHANDLED_WRITE(16, SPU, 3, 4);
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
        if (RAM_RANGE.contains(address, offset)) {
            m_RAM[offset + 0] = (data >>  0) & 0xFF;
            m_RAM[offset + 1] = (data >>  8) & 0xFF;
            m_RAM[offset + 2] = (data >> 16) & 0xFF;
            m_RAM[offset + 3] = (data >> 24) & 0xFF;
            return;
        }

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
                PRINT_UNHANDLED_WRITE(32, MEM_CTRL, 2, 8);
            }
            return;
        }

        if (RAM_SIZE_RANGE.contains(address, offset)) {
            PRINT_UNHANDLED_WRITE(32, RAM_SIZE, 2, 8);
            return;
        }

        if (IRQ_CTRL_RANGE.contains(address, offset)) {
            PRINT_UNHANDLED_WRITE(32, IRQ_CTRL, 2, 8);
            return;
        }

        if (DMA_RANGE.contains(address, offset)) {
            PRINT_UNHANDLED_WRITE(32, DMA, 2, 8);
            return;
        }

        if (CACHE_CTRL_RANGE.contains(address, offset)) {
            PRINT_UNHANDLED_WRITE(32, CACHE_CTRL, 2, 8);
            return;
        }

        std::cerr << "Unhandled write32 to memory at address: " << HEX(address, 8) << " data: " << HEX(data, 8) << '\n';
        assert(false);
    }

} // namespace PSX
