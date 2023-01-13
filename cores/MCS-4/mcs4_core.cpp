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
    fin.read((char*)(nullptr), 0x4A0);
    fin.close();
}

void MCS4Core::reset()
{
    m_cpu.reset();
}

void MCS4Core::clock()
{
    m_cpu.clock();
}

MCS4Core::MCS4Core() :
    m_cpu{ CPU4040::Mode::Intel4004 },
    m_windowSettings{ 64, 64, "MCS-4" }
{
}
