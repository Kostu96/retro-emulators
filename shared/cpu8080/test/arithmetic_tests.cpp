#include "instructions_tests_fixture.hpp"

using CPU8080ArithmeticTests = CPU8080Tests;

#pragma region BRegister

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

TEST_F(CPU8080ArithmeticTests, ADDBWithAuxCarryTest)
{
	rom[0x0] = 0x80;
	cpu.getState().A = 0xF;
	cpu.getState().B = 0x1;

	auto preExecutionState = captureCPUState();
	u8 cycles = 4;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x10;
	preExecutionState.F.AuxiliaryCarry = 1;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080ArithmeticTests, ADDBWithSignTest)
{
	rom[0x0] = 0x80;
	cpu.getState().A = 0x85;
	cpu.getState().B = 0x1;

	auto preExecutionState = captureCPUState();
	u8 cycles = 4;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x86;
	preExecutionState.F.Sign = 1;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080ArithmeticTests, ADDBWithZeroTest)
{
	rom[0x0] = 0x80;
	cpu.getState().A = 0xFF;
	cpu.getState().B = 0x1;

	auto preExecutionState = captureCPUState();
	u8 cycles = 4;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0;
	preExecutionState.F.Zero = 1;
	preExecutionState.F.Parity = 1;
	preExecutionState.F.AuxiliaryCarry = 1;
	preExecutionState.F.Carry = 1;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080ArithmeticTests, ADDBWithParityTest)
{
	rom[0x0] = 0x80;
	cpu.getState().A = 0x55;
	cpu.getState().B = 0x22;

	auto preExecutionState = captureCPUState();
	u8 cycles = 4;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x77;
	preExecutionState.F.Parity = 1;
	compareCPUStates(preExecutionState, postExecutionState);
}

#pragma endregion
#pragma region CRegister

TEST_F(CPU8080ArithmeticTests, ADDCStandardTest)
{
	rom[0x0] = 0x81;
	cpu.getState().A = 0x3;
	cpu.getState().C = 0x5;

	auto preExecutionState = captureCPUState();
	u8 cycles = 4;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x8;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080ArithmeticTests, ADDCWithCarryTest)
{
	rom[0x0] = 0x81;
	cpu.getState().A = 0xF7;
	cpu.getState().C = 0x82;

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

TEST_F(CPU8080ArithmeticTests, ADDCWithAuxCarryTest)
{
	rom[0x0] = 0x81;
	cpu.getState().A = 0xF;
	cpu.getState().C = 0x1;

	auto preExecutionState = captureCPUState();
	u8 cycles = 4;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x10;
	preExecutionState.F.AuxiliaryCarry = 1;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080ArithmeticTests, ADDCWithSignTest)
{
	rom[0x0] = 0x81;
	cpu.getState().A = 0x85;
	cpu.getState().C = 0x1;

	auto preExecutionState = captureCPUState();
	u8 cycles = 4;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x86;
	preExecutionState.F.Sign = 1;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080ArithmeticTests, ADDCWithZeroTest)
{
	rom[0x0] = 0x81;
	cpu.getState().A = 0xFF;
	cpu.getState().C = 0x1;

	auto preExecutionState = captureCPUState();
	u8 cycles = 4;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0;
	preExecutionState.F.Zero = 1;
	preExecutionState.F.Parity = 1;
	preExecutionState.F.AuxiliaryCarry = 1;
	preExecutionState.F.Carry = 1;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080ArithmeticTests, ADDCWithParityTest)
{
	rom[0x0] = 0x81;
	cpu.getState().A = 0x55;
	cpu.getState().C = 0x22;

	auto preExecutionState = captureCPUState();
	u8 cycles = 4;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x77;
	preExecutionState.F.Parity = 1;
	compareCPUStates(preExecutionState, postExecutionState);
}

#pragma endregion
#pragma region DRegister

TEST_F(CPU8080ArithmeticTests, ADDDStandardTest)
{
	rom[0x0] = 0x82;
	cpu.getState().A = 0x3;
	cpu.getState().D = 0x5;

	auto preExecutionState = captureCPUState();
	u8 cycles = 4;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x8;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080ArithmeticTests, ADDDWithCarryTest)
{
	rom[0x0] = 0x82;
	cpu.getState().A = 0xF7;
	cpu.getState().D = 0x82;

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

TEST_F(CPU8080ArithmeticTests, ADDDWithAuxCarryTest)
{
	rom[0x0] = 0x82;
	cpu.getState().A = 0xF;
	cpu.getState().D = 0x1;

	auto preExecutionState = captureCPUState();
	u8 cycles = 4;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x10;
	preExecutionState.F.AuxiliaryCarry = 1;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080ArithmeticTests, ADDDWithSignTest)
{
	rom[0x0] = 0x82;
	cpu.getState().A = 0x85;
	cpu.getState().D = 0x1;

	auto preExecutionState = captureCPUState();
	u8 cycles = 4;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x86;
	preExecutionState.F.Sign = 1;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080ArithmeticTests, ADDDWithZeroTest)
{
	rom[0x0] = 0x82;
	cpu.getState().A = 0xFF;
	cpu.getState().D = 0x1;

	auto preExecutionState = captureCPUState();
	u8 cycles = 4;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0;
	preExecutionState.F.Zero = 1;
	preExecutionState.F.Parity = 1;
	preExecutionState.F.AuxiliaryCarry = 1;
	preExecutionState.F.Carry = 1;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080ArithmeticTests, ADDDWithParityTest)
{
	rom[0x0] = 0x82;
	cpu.getState().A = 0x55;
	cpu.getState().D = 0x22;

	auto preExecutionState = captureCPUState();
	u8 cycles = 4;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x77;
	preExecutionState.F.Parity = 1;
	compareCPUStates(preExecutionState, postExecutionState);
}

#pragma endregion
