#include "instructions_tests_fixture.hpp"

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
