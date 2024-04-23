#include "c64.hpp"
#include "shared/source/address_range.hpp"
#include "shared/source/file_io.hpp"

#include <cassert>
#include <iostream>

namespace C64 {

    static constexpr AddressRange16 KERNAL_RANGE{ 0xE000, 0xFFFF };
    static constexpr AddressRange16 CIA2_RANGE{ 0xDD00, 0xDDFF };
    static constexpr AddressRange16 CIA1_RANGE{ 0xDC00, 0xDCFF };
    static constexpr AddressRange16 SID_RANGE{ 0xD400, 0xD7FF };
    static constexpr AddressRange16 VIC_RANGE{ 0xD000, 0xD3FF };
    static constexpr AddressRange16 BASIC_RANGE{ 0xA000, 0xBFFF };
    static constexpr AddressRange16 RAM_RANGE{ 0x0000, 0x9FFF };

    Emulator::Emulator()
    {
        size_t size = 0x2000;
        if (!readFile("rom/c64c/kernal.bin", (char*)m_KERNAL, size, true))
            std::cerr << "Could not read KERNAL ROM file!\n";

        size = 0x1000;
        if (!readFile("rom/c64c/characters.bin", (char*)m_characters, size, true))
            std::cerr << "Could not read characters ROM file!\n";

        size = 0x2000;
        if (!readFile("rom/c64c/basic.bin", (char*)m_BASIC, size, true))
            std::cerr << "Could not read BASIC ROM file!\n";

        m_cpu.mapReadMemoryCallback([this](u16 address) { return memoryRead(address); });
        m_cpu.mapWriteMemoryCallback([this](u16 address, u8 data) { return memoryWrite(address, data); });

        m_cpu.reset();
    }

    void Emulator::clock()
    {
        m_cpu.clock();
    }

    u8 Emulator::memoryRead(u16 address) const
    {
        u16 offset;

        if (KERNAL_RANGE.contains(address, offset)) {
            return ((m_cpuPORT & 3) > 1) ? m_KERNAL[offset] : m_RAM[address];
        }

        if (CIA2_RANGE.contains(address, offset)) return m_cia2.load8(offset);

        if (CIA1_RANGE.contains(address, offset)) return m_cia1.load8(offset);

        if (VIC_RANGE.contains(address, offset)) return m_vic.load8(offset);

        if (BASIC_RANGE.contains(address, offset)) return m_BASIC[offset];

        if (RAM_RANGE.contains(address, offset)) return m_RAM[offset];

        assert(false && "Unhandled memory read!");
        return 0;
    }

    void Emulator::memoryWrite(u16 address, u8 data)
    {
        if (address == 0x0000) m_cpuDDR = data;

        if (address == 0x0001) m_cpuPORT = data;

        u16 offset;
        if (KERNAL_RANGE.contains(address, offset)) {
            m_RAM[address] = data;
            return;
        }

        if (CIA2_RANGE.contains(address, offset)) {
            m_cia2.store8(offset, data);
            return;
        }

        if (CIA1_RANGE.contains(address, offset)) {
            m_cia1.store8(offset, data);
            return;
        }

        if (SID_RANGE.contains(address, offset)) {
            m_sid.store8(offset, data);
            return;
        }

        if (VIC_RANGE.contains(address, offset)) {
            m_vic.store8(offset, data);
            return;
        }

        if (BASIC_RANGE.contains(address, offset)) {
            m_RAM[address] = data;
            return;
        }

        if (RAM_RANGE.contains(address, offset)) {
            m_RAM[offset] = data;
            return;
        }

        assert(false && "Unhandled memory write!");
    }

} // namespace C64
