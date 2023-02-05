#include "cpu8080_gameboy_mode_fixture.hpp"

using CPU8080GameBoyModeArithmeticTests = CPU8080GameBoyModeTests;

TEST_F(CPU8080GameBoyModeArithmeticTests, INC_RPTest)
{
	rom[0x0] = 0x03; // INC BC
	rom[0x1] = 0x13; // INC DE
	rom[0x2] = 0x23; // INC HL
	rom[0x3] = 0x33; // INC SP

	CPU.m_state.BC = 0x0000;
	CPU.m_state.DE = 0x000F;
	CPU.m_state.HL = 0x00F0;
	CPU.m_state.SP = 0x0F00;
	auto preExecutionState = captureCPUState();
	int cycles = 4;
	while (cycles--)
		CPU.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0004;
	preExecutionState.BC = 0x0001;
	preExecutionState.DE = 0x0010;
	preExecutionState.HL = 0x00F1;
	preExecutionState.SP = 0x0F01;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080GameBoyModeArithmeticTests, DEC_RPTest)
{
	rom[0x0] = 0x0B; // DEC BC
	rom[0x1] = 0x1B; // DEC DE
	rom[0x2] = 0x2B; // DEC HL
	rom[0x3] = 0x3B; // DEC SP

	CPU.m_state.BC = 0x0002;
	CPU.m_state.DE = 0x000F;
	CPU.m_state.HL = 0x00F0;
	CPU.m_state.SP = 0x0F00;
	auto preExecutionState = captureCPUState();
	int cycles = 4;
	while (cycles--)
		CPU.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0004;
	preExecutionState.BC = 0x0001;
	preExecutionState.DE = 0x000E;
	preExecutionState.HL = 0x00EF;
	preExecutionState.SP = 0x0EFF;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080GameBoyModeArithmeticTests, ADD_HL_BCTest)
{
	rom[0x0] = 0x09; // ADD HL, BC

	CPU.m_state.HL = 0xDE00;
	CPU.m_state.BC = 0x00AD;

	auto preExecutionState = captureCPUState();
	CPU.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.HL = 0xDEAD;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080GameBoyModeArithmeticTests, ADD_HL_DETest)
{
	rom[0x0] = 0x19; // ADD HL, DE
	cpu.HL = 0xDE00;
	cpu.DE = 0x00AD;
	cpu.F.byte &= 0x0F;

	auto preExecutionState = captureCPUState();
	CPU.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC += 1;
	preExecutionState.HL = 0xDEAD;
	preExecutionState.AF |= 0x0;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080GameBoyModeArithmeticTests, ADD_HL_HLTest)
{
	rom[0x0] = 0x29; // ADD HL, HL
	cpu.HL = 0x4269;
	cpu.F.byte &= 0x0F;

	auto preExecutionState = captureCPUState();
	CPU.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC += 1;
	preExecutionState.HL = 0x84D2;
	preExecutionState.AF |= 0x0;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080GameBoyModeArithmeticTests, ADD_HL_SPTest)
{
	rom[0x0] = 0x39; // ADD HL, SP
	cpu.HL = 0xDE00;
	cpu.SP = 0x00AD;
	cpu.F.byte &= 0x0F;

	auto preExecutionState = captureCPUState();
	CPU.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC += 1;
	preExecutionState.HL = 0xDEAD;
	preExecutionState.AF |= 0x000F;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080GameBoyModeArithmeticTests, ADD_SP_Imm8Test)
{
	rom[0x0] = 0xE8;
	rom[0x1] = 0x42; // ADD SP, 0x42 (66)
	rom[0x1] = 0xE8;
	rom[0x2] = 0x42; // ADD SP, 0xBE (-66)
	cpu.SP = 0xDE00;
	cpu.F.byte &= 0x0F;

	auto preExecutionState = captureCPUState();
	CPU.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC += 2;
	preExecutionState.SP = 0xDE42;
	preExecutionState.AF |= 0x000F;
	compareCPUStates(preExecutionState, postExecutionState);
}