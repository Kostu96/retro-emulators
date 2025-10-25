#include "invaders.hpp"
#include "utils/address_range.hpp"
#include "utils/file_io.hpp"

#include <cassert>
#include <cstdio>
#include <cstring>

static const AddressRange16 ROM_RANGE{ 0x0000, 0x1FFF };
static const AddressRange16 RAM_RANGE{ 0x2000, 0x4000 };

void Invaders::reset()
{
    m_cpu.reset();
    runUntilNextInstruction();
}

void Invaders::clock()
{
    m_cpu.clock();
    m_video.clock();
}

void Invaders::runUntilNextInstruction()
{
    do {
        clock();
    } while (m_cpu.getCyclesLeft() > 0);
}

Invaders::Invaders() :
    m_video{ m_cpu, m_VRAM }
{
    constexpr size_t ROM_SIZE = 0x800;
    size_t offset = 0;
    for (auto filename : {
        "rom/invaders/invaders_h.bin",
        "rom/invaders/invaders_g.bin",
        "rom/invaders/invaders_f.bin",
        "rom/invaders/invaders_e.bin"
        })
    {
        size_t size = ROM_SIZE;
        [[maybe_unused]] bool ret = readFile(filename, reinterpret_cast<char*>(m_ROM + offset), size, true);
        assert(ret && "File read failed!");
        offset += ROM_SIZE;
    }

    m_cpu.mapReadMemoryCallback([this](u16 address) { return memoryRead(address); });
    m_cpu.mapWriteMemoryCallback([this](u16 address, u8 data) { memoryWrite(address, data); });
    m_cpu.mapReadIOCallback([this](u8 port) { return m_io.read8(port); });
    m_cpu.mapWriteIOCallback([this](u8 port, u8 data) { m_io.write8(port, data); });

    reset();
}

u8 Invaders::memoryRead(u16 address) const
{
    u16 offset;

    if (ROM_RANGE.contains(address, offset)) return m_ROM[offset];
    if (RAM_RANGE.contains(address, offset)) return m_RAM[offset];

    assert(false && "Unhandled memory read!");
    return 0xCD;
}

void Invaders::memoryWrite(u16 address, u8 data)
{
    u16 offset;

    if (RAM_RANGE.contains(address, offset)) {
        m_RAM[offset] = data;
        return;
    }

    assert(false && "Unkhandled memory write");
}
