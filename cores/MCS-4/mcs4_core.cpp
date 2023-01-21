#include "mcs4_core.hpp"
#include "disasm4040.hpp"

#include <cassert>
#include <cstring>
#include <fstream>

extern "C"
{
    __declspec(dllexport) MCS4Core* allocator()
    {
        return new MCS4Core{};
    }

    __declspec(dllexport) void deleter(MCS4Core* ptr)
    {
        delete ptr;
    }
}

static struct {
    u8 rom[0x100];

    u8 read(u16 address) const { return rom[address]; }
} ROM;

static struct {
    static constexpr u8 NUM_CHIPS = 4;
    u8 ram[32 * NUM_CHIPS];
    u8 status[8 * NUM_CHIPS];

    u8 read(u16 address) const {
        return address % 2 ? ram[address / 2] & 0xF : ram[address / 2] >> 4;
    }

    void write(u16 address, u8 data) {
        ram[address / 2] &= address % 2 ? 0xF0 : 0x0F;
        ram[address / 2] |= address % 2 ? data : data << 4;
    }

    u8 readStatus(u8 address) {
        return address % 2 ? status[address / 2] & 0xF : status[address / 2] >> 4;
    }

    void writeStatus(u8 address, u8 data) {
        status[address / 2] &= address % 2 ? 0xF0 : 0x0F;
        status[address / 2] |= address % 2 ? data : data << 4;
    }
} RAM;

u8 MCS4Core::getByteAt(u16 address, size_t memoryIndex) const
{
    switch (memoryIndex)
    {
    case 0: return m_cpu.loadROM8(address);
    case 1: return RAM.ram[address];
    case 2: return RAM.status[address];
    default: return 0xCD;
    }
}

void MCS4Core::loadROM(const char* filename)
{
    std::ifstream fin{ filename, std::ios::binary };
    fin.read((char*)(ROM.rom), sizeof(ROM.rom));
    fin.close();

    disassemble(ROM.rom, sizeof(ROM.rom), m_disassembly);
}

void MCS4Core::reset()
{
    m_cpu.reset();

    updateState();
}

void MCS4Core::update(double /*dt*/)
{
    m_cpu.clock();

    updateState();
}

MCS4Core::MCS4Core() :
    m_cpu{ CPU4040::Mode::Intel4004 },
    m_emulatorSettings{ 1, 1, 256, 128, "MCS-4" }
{
    m_cpu.map(ROM, { 0x0000, 0x00FF });
    m_cpu.map(RAM, { 0, 32 * 2 - 1 });
    m_cpu.mapReadROMIO([this](u8 chip) { return readROMIO(chip); });
    m_cpu.mapWriteROMIO([this](u8 chip, u8 data) { writeROMIO(chip, data); });
    m_cpu.mapWriteRAMOut([this](u8 chip, u8 data) { return writeRAMOut(chip, data); });
    m_cpu.mapReadRAMStatus([](u8 address) { return RAM.readStatus(address); });
    m_cpu.mapWriteRAMStatus([](u8 address, u8 data) { RAM.writeStatus(address, data); });

    m_state.push_back({ 0, 3, "Stack0", true });
    m_state.push_back({ 0, 1, "SP" });
    m_state.push_back({ 0, 3, "Stack1" });
    m_state.push_back({ 0, 3, "Stack2" });
    m_state.push_back({ 0, 3, "Stack3", false, true });
    m_state.push_back({ 0, 1, "ACC", true });
    m_state.push_back({ 0, 1, "CY", false, true });
    m_state.push_back({ 0, 2, "R0R1", true });
    m_state.push_back({ 0, 2, "R8R9" });
    m_state.push_back({ 0, 2, "R2R3", true });
    m_state.push_back({ 0, 2, "RARB" });
    m_state.push_back({ 0, 2, "R4R5", true });
    m_state.push_back({ 0, 2, "RCRD" });
    m_state.push_back({ 0, 2, "R6R7", true });
    m_state.push_back({ 0, 2, "RERF", false, true });
    m_state.push_back({ 0, 2, "SRCReg" });
    m_state.push_back({ 0, 1, "CMRAM" });
}

void MCS4Core::updateState()
{
    m_state[0].value = m_cpu.getStack()[0];
    m_state[1].value = m_cpu.getSP();
    m_state[2].value = m_cpu.getStack()[1];
    m_state[3].value = m_cpu.getStack()[2];
    m_state[4].value = m_cpu.getStack()[3];
    m_state[5].value = m_cpu.getACC();
    m_state[6].value = m_cpu.getCY();
    m_state[7].value = m_cpu.getRegs()[0];
    m_state[8].value = m_cpu.getRegs()[4];
    m_state[9].value = m_cpu.getRegs()[1];
    m_state[10].value = m_cpu.getRegs()[5];
    m_state[11].value = m_cpu.getRegs()[2];
    m_state[12].value = m_cpu.getRegs()[6];
    m_state[13].value = m_cpu.getRegs()[3];
    m_state[14].value = m_cpu.getRegs()[7];
    m_state[15].value = m_cpu.getSRCReg();
    m_state[16].value = m_cpu.getCMRAM();
}

u8 MCS4Core::readROMIO(u8 chip)
{
    return 0xA;
}

void MCS4Core::writeROMIO(u8 /*chip*/, u8 /*data*/)
{

}

void MCS4Core::writeRAMOut(u8 /*chip*/, u8 /*data*/)
{

}
