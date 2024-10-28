#include "cpu8080_tests_fixture.hpp"

using CPU8080ArithmeticTests = CPU8080Tests;

TEST_F(CPU8080ArithmeticTests, ADDBStandardTest)
{
	rom[0x0] = 0x80;
	cpu.getState().A = 0x3;
	cpu.getState().B = 0x5;

	auto preExecutionState = captureCPUState();
	u8 cycles = 4;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x8;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080ArithmeticTests, ADDBWithCarryTest)
{
	rom[0x0] = 0x80;
	cpu.getState().A = 0xF7;
	cpu.getState().B = 0x82;

	auto preExecutionState = captureCPUState();
	u8 cycles = 4;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x79;
	preExecutionState.F.Carry = 1;
	compareCPUStates(preExecutionState, postExecutionState);
}
