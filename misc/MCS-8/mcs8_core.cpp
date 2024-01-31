#include "mcs8_core.hpp"
#include "asm8008.hpp"

#include <cassert>
#include <cstring>
#include <fstream>

extern "C"
{
    __declspec(dllexport) MCS8Core* allocator()
    {
        return new MCS8Core{};
    }

    __declspec(dllexport) void deleter(MCS8Core* ptr)
    {
        delete ptr;
    }
}

static struct {
    u8 rom[0x800];

    u8 read(u16 address) const { return rom[address]; }
} ROM;

static struct {
    u8 ram[0x400];

    u8 read(u16 address) const { return ram[address]; }

    void write(u16 address, u8 data) { ram[address] = data; }
} RAM;

u8 MCS8Core::getByteAt(u16 address, size_t memoryIndex) const
{
    return 0xCD;
}

void MCS8Core::loadROM(const char* filename)
{
    std::ifstream fin{ filename, std::ios::binary };
    fin.read((char*)(ROM.rom), sizeof(ROM.rom));
    fin.close();

    ASM8008::disassemble(ROM.rom, sizeof(ROM.rom), m_disassembly);
}

void MCS8Core::reset()
{
    m_cpu.reset();

    updateState();
}

void MCS8Core::update(double /*dt*/)
{
    m_cpu.clock();

    updateState();
}

MCS8Core::MCS8Core() :
    m_emulatorSettings{ 1, 1, 256, 128, "MCS-8" }
{
    m_cpu.map(ROM, { 0x000, 0x7FF });
    m_cpu.map(RAM, { 0x800, 0xBFF });

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

void MCS8Core::updateState()
{
    /*m_state[0].value = m_cpu.getStack()[0];
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
    m_state[16].value = m_cpu.getCMRAM();*/
}
