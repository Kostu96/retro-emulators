#include "cpu8080.hpp"

#include <gtest/gtest.h>

struct CPU8080Tests :
    public testing::Test
{
    static constexpr u16 ROM_SIZE = 0x10;
    static constexpr u16 RAM_SIZE = 0x10;

    struct State {
        u16 PC;
        u16 SP;
        u16 AF;
        u16 BC;
        u16 DE;
        u16 HL;
    };

    u8 rom[ROM_SIZE]{};
    u8 ram[RAM_SIZE]{};

    CPU8080 cpu;
    State referenceState;

    CPU8080Tests()
    {
        cpu.mapReadMemoryCallback([&](u16 address)
            {
                if (address < ROM_SIZE) return rom[address];
                if (address >= 0x8000) return ram[address - 0x8000];
                return u8{};
            });

        cpu.mapWriteMemoryCallback([&](u16 address, u8 data)
            {
                if (address >= 0x8000) ram[address - 0x8000] = data;
            });
    }

    void SetUp() override
    {
        cpu.reset();

        referenceState.PC = cpu
    }

    void TearDown() override
    {
        EXPECT_EQ(referenceState.PC, cpu.getPC());
        EXPECT_EQ(referenceState.SP, cpu.getSP());
        EXPECT_EQ(referenceState.AF, cpu.getAF());
        EXPECT_EQ(referenceState.BC, cpu.getBC());
        EXPECT_EQ(referenceState.DE, cpu.getDE());
        EXPECT_EQ(referenceState.HL, cpu.getHL());
    }
};

TEST_F(CPU8080Tests, NOPTest)
{
    rom[0x0] = 0x00; // NOP

    u8 clocks = 1;
    while (clocks--)
        cpu.clock();

    referenceCPUState.PC = 0x0001;
}

TEST_F(CPU8080Tests, JMPTest)
{
    rom[0x0] = 0xC3;
    rom[0x1] = 0xAD;
    rom[0x2] = 0xDE; // JMP 0xDEAD

    u8 clocks = 1;
    while (clocks--)
        cpu.clock();

    referenceCPUState.PC = 0xDEAD;
}

TEST_F(CPU8080Tests, LXITest)
{
    rom[0x0] = 0x01;
    rom[0x1] = 0x23;
    rom[0x2] = 0x01; // LXI BC, 0x0123

    rom[0x3] = 0x11;
    rom[0x4] = 0x67;
    rom[0x5] = 0x45; // LXI DE, 0x4567

    rom[0x6] = 0x21;
    rom[0x7] = 0xAB;
    rom[0x8] = 0x89; // LXI HL, 0x89AB

    rom[0x9] = 0x31;
    rom[0xA] = 0xEF;
    rom[0xB] = 0xCD; // LXI SP, 0xCDEF

    u8 clocks = 4;
    while (clocks--)
        cpu.clock();

    referenceCPUState.BC = 0x0123;
    referenceCPUState.DE = 0x4567;
    referenceCPUState.HL = 0x89AB;
    referenceCPUState.SP = 0xCDEF;
    referenceCPUState.PC = 0x000C;
}

TEST_F(CPUx80ModeI8080Tests, MVITest)
{
    rom[0x0] = 0x06;
    rom[0x1] = 0x01; // MVI B, 0x01

    rom[0x2] = 0x0E;
    rom[0x3] = 0x23; // MVI C, 0x23

    rom[0x4] = 0x16;
    rom[0x5] = 0x45; // MVI D, 0x45

    rom[0x6] = 0x1E;
    rom[0x7] = 0x67; // MVI E, 0x67

    rom[0x8] = 0x26;
    rom[0x9] = 0x80; // MVI H, 0x80

    rom[0xA] = 0x2E;
    rom[0xB] = 0x01; // MVI L, 0x01

    rom[0xC] = 0x36;
    rom[0xD] = 0xCD; // MVI [HL], 0xCD

    rom[0xE] = 0x3E;
    rom[0xF] = 0xEF; // MVI A, 0xEF

    u8 clocks = 8;
    while (clocks--)
        cpu.clock();

    referenceCPUState.BC = 0x0123;
    referenceCPUState.DE = 0x4567;
    referenceCPUState.HL = 0x8001;
    referenceCPUState.A = 0xEF;
    referenceCPUState.PC = 0x0010;

    EXPECT_EQ(ram[1], 0xCD);
}
