#include "memory_map.h"

#include <iostream>

uint8_t MemoryMap::load8(uint32_t address) const
{
    uint32_t absAddr = maskRegion(address);
    uint32_t offset;

    if (BIOS_RANGE.contains(absAddr, offset))
        return m_bios.load8(offset);

    if (RAM_RANGE.contains(absAddr, offset))
        return m_ram.load8(offset);

    if (CDROM_RANGE.contains(absAddr, offset))
        return m_cdROM.load8(offset);

    if (EXPANSION1_RANGE.contains(absAddr, offset))
        return 0xFF; // To indicate that there is no EXPANSION1

    std::cerr << "Unhandled access when loading from: " << std::hex << address << std::dec << '\n';
    __debugbreak();
    return 0xDE;
}

uint16_t MemoryMap::load16(uint32_t address) const
{
    if (address % 2 != 0) {
        std::cerr << "Unaligned access when loading from: " << std::hex << address << std::dec << '\n';
        abort();
    }

    uint32_t absAddr = maskRegion(address);
    uint32_t offset;

    if (RAM_RANGE.contains(absAddr, offset))
        return m_ram.load16(offset);

    if (SPU_RANGE.contains(absAddr, offset)) {
        //std::cerr << "Temp handled read from SPU\n";
        return 0;
    }

    if (IRQ_CONTROL_RANGE.contains(absAddr, offset)) {
        std::cerr << "Temp handled load from IRQ_CONTROL\n";
        return 0;
    }

    std::cerr << "Unhandled access when loading from: " << std::hex << address << std::dec << '\n';
    __debugbreak();
    return 0xDEAD;
}

uint32_t MemoryMap::load32(uint32_t address) const
{
    if (address % 4 != 0) {
        std::cerr << "Unaligned access when loading from: " << std::hex << address << std::dec << '\n';
        abort();
    }

    uint32_t absAddr = maskRegion(address);
    uint32_t offset;
    
    if (BIOS_RANGE.contains(absAddr, offset))
        return m_bios.load32(offset);

    if (RAM_RANGE.contains(absAddr, offset))
        return m_ram.load32(offset);

    if (DMA_RANGE.contains(absAddr, offset))
        return m_dma.load32(offset);

    if (GPU_RANGE.contains(absAddr, offset))
        return m_gpu.load32(offset);

    if (IRQ_CONTROL_RANGE.contains(absAddr, offset)) {
        std::cerr << "Temp handled load from IRQ_CONTROL\n";
        return 0;
    }

    if (TIMERS_RANGE.contains(absAddr, offset))
        return m_timers.load32(offset);

    std::cerr << "Unhandled access when loading from: " << std::hex << address << std::dec << '\n';
    __debugbreak();
    return 0xDEADBEEF;
}

void MemoryMap::store8(uint32_t address, uint8_t value)
{
    uint32_t absAddr = maskRegion(address);
    uint32_t offset;

    if (RAM_RANGE.contains(absAddr, offset)) {
        m_ram.store8(offset, value);
        return;
    }

    if (CDROM_RANGE.contains(absAddr, offset)) {
        m_cdROM.store8(offset, value);
        return;
    }

    if (EXPANSION2_RANGE.contains(absAddr, offset)) {
        std::cerr << "Unhandled store8 to: " << std::hex << address <<
                     " EXPANSION2(" << offset << "): " << (uint16_t)value << std::dec << '\n';
        return;
    }

    std::cerr << "Unhandled access when storing to: " << std::hex << address << std::dec << '\n';
    __debugbreak();
}

void MemoryMap::store16(uint32_t address, uint16_t value)
{
    if (address % 2 != 0) {
        std::cerr << "Unaligned access when storing to: " << std::hex << address << std::dec << '\n';
        abort();
    }

    uint32_t absAddr = maskRegion(address);
    uint32_t offset;

    if (RAM_RANGE.contains(absAddr, offset)) {
        m_ram.store16(offset, value);
        return;
    }

    if (SPU_RANGE.contains(absAddr, offset)) {
        //std::cerr << "Unhandled write to SPU: " << std::hex << address << std::dec << '\n';
        return; // Ignore SPU for now
    }

    if (IRQ_CONTROL_RANGE.contains(absAddr, offset)) {
        std::cerr << "Unhandled write to IRQ_CONTROL: " << std::hex << address << std::dec << '\n';
        return; // Ignore stores to IRQ_CONTROL
    }

    if (TIMERS_RANGE.contains(absAddr, offset)) {
        m_timers.store16(offset, value);
        return;
    }

    std::cerr << "Unhandled access when storing to: " << std::hex << address << std::dec << '\n';
    __debugbreak();
}

void MemoryMap::store32(uint32_t address, uint32_t value)
{
    if (address % 4 != 0) {
        std::cerr << "Unaligned access when storing to: " << std::hex << address << std::dec << '\n';
        abort();
    }

    uint32_t absAddr = maskRegion(address);
    uint32_t offset;

    if (RAM_RANGE.contains(absAddr, offset)) {
        m_ram.store32(offset, value);
        return;
    }

    if (DMA_RANGE.contains(absAddr, offset)) {
        DMA::Port activePort = DMA::Port::Invalid;
        m_dma.store32(offset, value, activePort);
        if (activePort != DMA::Port::Invalid)
            DMATransfer(activePort);
        return;
    }

    if (GPU_RANGE.contains(absAddr, offset)) {
        m_gpu.store32(offset, value);
        return;
    }

    if (IRQ_CONTROL_RANGE.contains(absAddr, offset)) {
        std::cerr << "Unhandled store32 to: " << std::hex << address <<
            " IRQ_CONTROL(" << offset << "): " << value << std::dec << '\n';
        return;
    }

    if (TIMERS_RANGE.contains(absAddr, offset)) {
        m_timers.store32(offset, value);
        return;
    }

    if (CACHE_CONTROL_RANGE.contains(absAddr, offset)) {
        std::cerr << "Unhandled store32 to: " << std::hex << address <<
                     " CACHE_CONTROL(" << offset << "): " << value << std::dec << '\n';
        return;
    }

    if (MEMORY_CONTROL_RANGE.contains(absAddr, offset)) {
        switch (offset) {
        case 0:
            if (value != 0x1F000000) {
                std::cerr << "Bad write to expansion 1 base address!\n";
                abort();
            }
            break;
        case 4:
            if (value != 0x1F802000) {
                std::cerr << "Bad write to expansion 2 base address!\n";
                abort();
            }
            break;
        default:
            std::cerr << "Unhandled store32 to: " << std::hex << address <<
                         " MEMORY_CONTROL(" << offset << "): " << value << std::dec << '\n';
        }
        return;
    }

    if (RAM_SIZE_RANGE.contains(absAddr, offset)) {
        std::cerr << "Unhandled store32 to: " << std::hex << address <<
                     " RAM_SIZE(" << offset << "): " << value << std::dec << '\n';
        return;
    }
    
    std::cerr << "Unhandled access when storing to: " << std::hex << address << std::dec << '\n';
    __debugbreak();
}

uint32_t MemoryMap::maskRegion(uint32_t address)
{
    uint32_t index = address >> 29;
    return address & REGION_MASK[index];
}

void MemoryMap::DMATransfer(DMA::Port port)
{
    switch (m_dma.getChannel(port).control.fields.sync) {
    case DMA::Channel::Sync::Manual:
    case DMA::Channel::Sync::Request:
        DMATransferBlock(port);
        break;
    case DMA::Channel::Sync::LinkedList:
        DMATransferLinkedList(port);
        break;
    }
}

void MemoryMap::DMATransferBlock(DMA::Port port)
{
    DMA::Channel& channel = m_dma.getChannel(port);
    uint32_t address = channel.baseAddress.fields.address;
    uint32_t transferSize = channel.getTransferSize();

    while (transferSize > 0)
    {
        uint32_t currentAddress = address & 0x1FFFFC;
        uint32_t word = 0xDEADBEEF;
        switch (channel.control.fields.direction) {
        case DMA::Channel::Direction::FromRAM: {
            word = m_ram.load32(currentAddress);
            switch (port) {
            case DMA::Port::GPU:
                m_gpu.store32(0, word); // send to GP0
                break;
            default:
                std::cerr << "Unimplemented DMA transfer target!\n";
                abort();
            }
        } break;
        case DMA::Channel::Direction::ToRAM: {
            switch (port) {
            case DMA::Port::OTC:
                if (transferSize == 1)
                    word = 0xFFFFFF;
                else
                    word = (address - 4) & 0x1FFFFF;
                break;
            case DMA::Port::GPU:
                std::cerr << "Unexpected DMA from GPU to RAM - address: " << std::hex << address << std::dec << '\n';
                break;
            default:
                std::cerr << "Unimplemented DMA transfer source!\n";
                abort();
            }
            m_ram.store32(currentAddress, word);
        } break;
        }

        address += channel.control.fields.step == DMA::Channel::Step::Increment ? 4 : -4;
        transferSize -= 1;
    }
    channel.done();
}

void MemoryMap::DMATransferLinkedList(DMA::Port port)
{
    DMA::Channel& channel = m_dma.getChannel(port);
    uint32_t address = channel.baseAddress.fields.address & 0x1FFFFC;

    if (channel.control.fields.direction == DMA::Channel::Direction::ToRAM) {
        std::cerr << "Invalid drirection form linked list DMA transfer!\n";
        abort();
    }

    if (port != DMA::Port::GPU) {
        std::cerr << "Linked list DMA transfer on port different than GPU!\n";
        abort();
    }

    uint32_t header = 0xDEADBEEF;
    do {
        header = m_ram.load32(address);
        uint32_t transferSize = header >> 24;
        while (transferSize > 0) {
            address += 4;
            address &= 0x1FFFFC;
            uint32_t command = m_ram.load32(address);
            m_gpu.store32(0, command); // send to GP0
            transferSize -= 1;
        }
        address = header & 0x1FFFFC;
    } while ((header & 0x800000) == 0);
    channel.done();
}

const uint32_t MemoryMap::REGION_MASK[] = {
    // KUSEG 2048MB
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    // KSEG0 512MB
    0x7FFFFFFF,
    // KSEG1 512MB
    0x1FFFFFFF,
    // KSEG2 1024MB
    0xFFFFFFFF, 0xFFFFFFFF
};
const AddressRange MemoryMap::RAM_RANGE{            0x00000000, 2 * 1024 * 1024 };
const AddressRange MemoryMap::EXPANSION1_RANGE{     0x1F000000, 8 * 1024 * 1024 };
const AddressRange MemoryMap::MEMORY_CONTROL_RANGE{ 0x1F801000, 36 };
const AddressRange MemoryMap::RAM_SIZE_RANGE{       0x1F801060, 4 };
const AddressRange MemoryMap::IRQ_CONTROL_RANGE{    0x1F801070, 8 };
const AddressRange MemoryMap::DMA_RANGE{            0x1F801080, 128 };
const AddressRange MemoryMap::TIMERS_RANGE{         0x1F801100, 48 };
const AddressRange MemoryMap::CDROM_RANGE{          0x1F801800, 4 };
const AddressRange MemoryMap::GPU_RANGE{            0x1F801810, 8 };
const AddressRange MemoryMap::SPU_RANGE{            0x1F801C00, 640 };
const AddressRange MemoryMap::EXPANSION2_RANGE{     0x1F802000, 128 };
const AddressRange MemoryMap::BIOS_RANGE{           0x1FC00000, 512 * 1024 };
const AddressRange MemoryMap::CACHE_CONTROL_RANGE{  0xFFFE0130, 4 };
