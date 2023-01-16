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
    u8 rom[256];

    u8 read(u16 address) const { return rom[address]; }
} ROM;

void MCS4Core::render(CharVertex* /*verts*/)
{
    
}

void MCS4Core::handleKey(int /*key*/, int /*action*/)
{
    constexpr int KEY1 = 49;
    constexpr int KEY2 = 50;
    constexpr int KEY3 = 51;
    constexpr int KEY4 = 52;
    constexpr int KEYQ = 81;
    constexpr int KEYW = 87;
    constexpr int KEYE = 69;
    constexpr int KEYR = 82;
    constexpr int KEYA = 65;
    constexpr int KEYS = 83;
    constexpr int KEYD = 68;
    constexpr int KEYF = 70;
    constexpr int KEYZ = 90;
    constexpr int KEYX = 88;
    constexpr int KEYC = 67;
    constexpr int KEYV = 86;
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

void MCS4Core::clock()
{
    m_cpu.clock();

    updateState();
}

MCS4Core::MCS4Core() :
    m_cpu{ CPU4040::Mode::Intel4004 },
    m_windowSettings{ 256, 128, "MCS-4" }
{
    m_cpu.map(ROM, { 0x0000, 0x00FF });
    m_cpu.mapReadROMIO([this](u8 chip) { return readROMIO(chip); });

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
    m_state.push_back({ 0, 2, "RAMAddr" });
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
    m_state[15].value = m_cpu.getRAMAddr();
}

u8 MCS4Core::readROMIO(u8 chip)
{
    return 0xA;
}
