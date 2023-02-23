#include "invaders_core.hpp"

#include <cassert>
#include <cstring>
#include <fstream>

//#define CPU_DIAG

static struct
{
    u8 rom[0x2000];

    u8 read(u16 address) const { return rom[address]; }
} ROM;

static struct
{
    u8 ram[0x400];

    u8 read(u16 address) const { return ram[address]; }
    void write(u16 address, u8 data) { ram[address] = data; }
} RAM;

static struct
{
    u8 screen[0x1C00];

    u8 read(u16 address) const { return screen[address]; }
    void write(u16 address, u8 data) { screen[address] = data; }
} Screen;

extern "C"
{
    __declspec(dllexport) InvadersCore* allocator()
    {
        return new InvadersCore{};
    }

    __declspec(dllexport) void deleter(InvadersCore* ptr)
    {
        delete ptr;
    }
}

u8 InvadersCore::getByteAt(u16 address, size_t memoryIndex) const
{
    return 0xCD;
}

void InvadersCore::handleKey(int key, int action)
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

void InvadersCore::reset()
{
    m_cpu.reset();

    updateState();
}

#include <cstdio>

void InvadersCore::update(double dt)
{
#ifdef CPU_DIAG
    if (m_cpu.getPC() == 0x0005)
    {
        u8 C = m_cpu.getBC() & 0xFF;
        u16 DE = m_cpu.getDE();

        if (C == 0x9)
        {
            char* str = (char*)&ROM.rom[DE];
            while (*str != '$')
                printf("%c", *str++);
        }
        else if (C == 0x2)
        {
            printf("%c", DE & 0xF);
        }
    }
#else
    if (m_cpu.getPC() == 0x1A5C)
    {
        std::memset(Screen.screen, 0, 0x1C00);
    }
#endif

    static double timer = 0.0;
    timer += dt;

    if (timer > 1.0 / 60.0)
    {
        timer -= 1.0 / 60.0;
        m_cpu.interrupt(2);

        u16 x = 0, y = 0;
        for (u16 i = 0; i < 0x1C00; i++)
        {
            u8 byte = Screen.screen[i];
            for (u16 bit = 0; bit < 8; bit++)
            {
                bool on = byte & (1 << (7 - bit));
                m_renderPoint(x, y, on ? 0xFFFFFFFF : 0);
            }

            x++;
            if (x >= 256)
            {
                x = 0;
                y++;
            }
        }
    }

    m_cpu.clock();

    updateState();
}

InvadersCore::InvadersCore() :
    m_cpu{ CPUx80Mode::Intel8080 },
    m_emulatorSettings{ 256, 224, 256 * 2, 224 * 2, "Space Invaders" }
{
#ifdef CPU_DIAG
    std::ifstream file{ "C:\\Users\\kmisiak\\myplace\\retro-extras\\programs\\8080\\cpudiag.bin", std::ios_base::binary };
    assert(file.is_open());
    file.read((char*)(ROM.rom + 0x0100), 0x5A7);
    file.read((char*)RAM.ram, 0x1);
    file.close();

    ROM.rom[0x0000] = 0xC3;
    ROM.rom[0x0001] = 0x00;
    ROM.rom[0x0002] = 0x01;

    ROM.rom[0x0170] = 0x07;

    ROM.rom[0x0005] = 0xC9; // Early out from syscall

    m_cpu.map(ROM, { 0x0000, 0x06A5 });
    m_cpu.map(RAM, { 0x06A6, 0x16A5 });
#else
    // TODO: change to config file library
    std::ifstream cfgFile{ "config.txt" };
    assert(cfgFile.is_open());

    std::string line;
    while (std::getline(cfgFile, line))
    {
        if (line[0] == '#') continue;
        if (line.compare(0, 21, "SpaceInvadersERomPath") == 0)
        {
            std::string path = line.substr(25, line.size() - 25 - 1);
            std::ifstream romFile{ path, std::ios_base::binary };
            assert(romFile.is_open());
            romFile.read((char*)(ROM.rom + 0x1800), 0x800);
            romFile.close();
        }
        else if (line.compare(0, 21, "SpaceInvadersFRomPath") == 0)
        {
            std::string path = line.substr(25, line.size() - 25 - 1);
            std::ifstream romFile{ path, std::ios_base::binary };
            assert(romFile.is_open());
            romFile.read((char*)(ROM.rom + 0x1000), 0x800);
            romFile.close();
        }
        else if (line.compare(0, 21, "SpaceInvadersGRomPath") == 0)
        {
            std::string path = line.substr(25, line.size() - 25 - 1);
            std::ifstream romFile{ path, std::ios_base::binary };
            assert(romFile.is_open());
            romFile.read((char*)(ROM.rom + 0x0800), 0x800);
            romFile.close();
        }
        else if (line.compare(0, 21, "SpaceInvadersHRomPath") == 0)
        {
            std::string path = line.substr(25, line.size() - 25 - 1);
            std::ifstream romFile{ path, std::ios_base::binary };
            assert(romFile.is_open());
            romFile.read((char*)(ROM.rom + 0x0000), 0x800);
            romFile.close();
        }
    }
    cfgFile.close();

    ROM.rom[0x1A5C] = 0xC9; // Early out from ClearScreen routine

    /*m_cpu.map(ROM, { 0x0000, 0x1FFF });
    m_cpu.map(RAM, { 0x2000, 0x23FF });*/
#endif

    //m_cpu.map(Screen, { 0x2400, 0x3FFF });

    disassemble(ROM.rom, 0x2000, m_disassembly);

    m_state.push_back({ 0, 4, "AF" });
    m_state.push_back({ 0, 4, "BC" });
    m_state.push_back({ 0, 4, "DE" });
    m_state.push_back({ 0, 4, "HL" });
    m_state.push_back({ 0, 4, "SP" });
    m_state.push_back({ 0, 4, "PC" });
}

void InvadersCore::updateState()
{
    m_state[0].value = m_cpu.getAF();
    m_state[1].value = m_cpu.getBC();
    m_state[2].value = m_cpu.getDE();
    m_state[3].value = m_cpu.getHL();
    m_state[4].value = m_cpu.getSP();
    m_state[5].value = m_cpu.getPC();
}
