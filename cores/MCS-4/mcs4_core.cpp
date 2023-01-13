#include "mcs4_core.hpp"

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

    m_state.push_back({ 0, 3, "Stack0" });
    m_state.push_back({ 0, 3, "Stack1" });
    m_state.push_back({ 0, 3, "Stack2" });
    m_state.push_back({ 0, 3, "Stack3" });
    m_state.push_back({ 0, 1, "SP" });
    m_state.push_back({ 0, 1, "ACC" });
    m_state.push_back({ 0, 1, "CY" });
    m_state.push_back({ 0, 2, "R0R1" });
    m_state.push_back({ 0, 2, "R2R3" });
    m_state.push_back({ 0, 2, "R4R5" });
    m_state.push_back({ 0, 2, "R6R7" });
}

void MCS4Core::updateState()
{
    m_state[0].value = m_cpu.getStack()[0];
    m_state[1].value = m_cpu.getStack()[1];
    m_state[2].value = m_cpu.getStack()[2];
    m_state[3].value = m_cpu.getStack()[3];
    m_state[4].value = m_cpu.getSP();
    m_state[5].value = m_cpu.getACC();
    m_state[6].value = m_cpu.getCY();
    m_state[7].value = m_cpu.getRegs()[0];
    m_state[8].value = m_cpu.getRegs()[1];
    m_state[9].value = m_cpu.getRegs()[2];
    m_state[10].value = m_cpu.getRegs()[3];
}
