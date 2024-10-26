#include "cpu8080_tests_fixture.hpp"

TEST_F(CPU8080Tests, NOPTest)
{
    rom[0x0] = 0x00; // NOP

    auto preExecutionState = captureCPUState();
    u8 clocks = 4;
    while (clocks--)
        cpu.clock();
    const auto postExecutionState = captureCPUState();

    preExecutionState.PC = 0x0001;
    compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080Tests, JMPTest)
{
    rom[0x0] = 0xC3;
    rom[0x1] = 0xAD;
    rom[0x2] = 0xDE; // JMP 0xDEAD

    auto preExecutionState = captureCPUState();
    u8 clocks = 10;
    while (clocks--)
        cpu.clock();
    const auto postExecutionState = captureCPUState();

    preExecutionState.PC = 0xDEAD;
    compareCPUStates(preExecutionState, postExecutionState);
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

    auto preExecutionState = captureCPUState();
    u8 clocks = 4 * 10;
    while (clocks--)
        cpu.clock();
    const auto postExecutionState = captureCPUState();

    preExecutionState.BC = 0x0123;
    preExecutionState.DE = 0x4567;
    preExecutionState.HL = 0x89AB;
    preExecutionState.SP = 0xCDEF;
    preExecutionState.PC = 0x000C;
    compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080Tests, MVITest)
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

    auto preExecutionState = captureCPUState();
    u8 clocks = 7 * 7 + 10;
    while (clocks--)
        cpu.clock();
    const auto postExecutionState = captureCPUState();

    preExecutionState.BC = 0x0123;
    preExecutionState.DE = 0x4567;
    preExecutionState.HL = 0x8001;
    preExecutionState.A = 0xEF;
    preExecutionState.PC = 0x0010;
    compareCPUStates(preExecutionState, postExecutionState);

    EXPECT_EQ(ram[1], 0xCD);
}
