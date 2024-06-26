#include "cpu_x80_gameboy_mode_fixture.hpp"

using CPUx80GameBoyModeArithmeticTests = CPUx80GameBoyModeTests;

TEST_F(CPUx80GameBoyModeArithmeticTests, INC_RPTest)
{
	rom[0x0] = 0x03; // INC BC
	rom[0x1] = 0x13; // INC DE
	rom[0x2] = 0x23; // INC HL
	rom[0x3] = 0x33; // INC SP

	cpu.m_state.BC = 0x0000;
	cpu.m_state.DE = 0x000F;
	cpu.m_state.HL = 0x00F0;
	cpu.m_state.SP = 0x0F00;
	auto preExecutionState = captureCPUState();
	u8 cycles = 4 * 2;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0004;
	preExecutionState.BC = 0x0001;
	preExecutionState.DE = 0x0010;
	preExecutionState.HL = 0x00F1;
	preExecutionState.SP = 0x0F01;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeArithmeticTests, DEC_RPTest)
{
	rom[0x0] = 0x0B; // DEC BC
	rom[0x1] = 0x1B; // DEC DE
	rom[0x2] = 0x2B; // DEC HL
	rom[0x3] = 0x3B; // DEC SP

	cpu.m_state.BC = 0x0002;
	cpu.m_state.DE = 0x000F;
	cpu.m_state.HL = 0x00F0;
	cpu.m_state.SP = 0x0F00;
	auto preExecutionState = captureCPUState();
	u8 cycles = 4 * 2;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0004;
	preExecutionState.BC = 0x0001;
	preExecutionState.DE = 0x000E;
	preExecutionState.HL = 0x00EF;
	preExecutionState.SP = 0x0EFF;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeArithmeticTests, ADD_HL_BCTest)
{
	rom[0x0] = 0x09; // ADD HL, BC

	cpu.m_state.HL = 0xDE00;
	cpu.m_state.BC = 0x00AD;

	auto preExecutionState = captureCPUState();
	u8 cycles = 2;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.HL = 0xDEAD;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeArithmeticTests, ADD_HL_DETest)
{
	rom[0x0] = 0x19; // ADD HL, DE

	cpu.m_state.HL = 0xDE00;
	cpu.m_state.DE = 0x00AD;

	auto preExecutionState = captureCPUState();
	u8 cycles = 2;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.HL = 0xDEAD;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeArithmeticTests, ADD_HL_HLTest)
{
	rom[0x0] = 0x29; // ADD HL, HL

	cpu.m_state.HL = 0x4269;

	auto preExecutionState = captureCPUState();
	u8 cycles = 2;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.HL = 0x84D2;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeArithmeticTests, ADD_HL_SPTest)
{
	rom[0x0] = 0x39; // ADD HL, SP

	cpu.m_state.HL = 0xDE00;
	cpu.m_state.SP = 0x00AD;

	auto preExecutionState = captureCPUState();
	u8 cycles = 2;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.HL = 0xDEAD;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeArithmeticTests, ADD_SP_Imm8Test)
{
	rom[0x0] = 0xE8;
	rom[0x1] = 0x42; // ADD SP, 0x42 (66)
	rom[0x2] = 0xE8;
	rom[0x3] = 0xBE; // ADD SP, 0xBE (-66)

	cpu.m_state.SP = 0xDE00;

	auto preExecutionState = captureCPUState();
	u8 cycles = 4;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0002;
	preExecutionState.SP = 0xDE42;
	compareCPUStates(preExecutionState, postExecutionState);

	preExecutionState = captureCPUState();
	cycles = 4;
	while (cycles--)
		cpu.clock();
	postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0004;
	preExecutionState.SP = 0xDE00;
	preExecutionState.F.HalfCarry = 1;
	preExecutionState.F.Carry = 1;
	compareCPUStates(preExecutionState, postExecutionState);
}
