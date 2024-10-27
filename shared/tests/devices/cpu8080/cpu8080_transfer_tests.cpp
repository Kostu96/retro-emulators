#include "cpu8080_tests_fixture.hpp"

using CPU8080TransferTests = CPU8080Tests;

TEST_F(CPU8080TransferTests, MOV_B_BTest)
{
	rom[0x0] = 0x40; // MOV B, B

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_B_CTest)
{
	rom[0x0] = 0x41; // MOV B, C
	cpu.getState().C = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.B = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_B_DTest)
{
	rom[0x0] = 0x42; // MOV B, D
	cpu.getState().D = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.B = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_B_ETest)
{
	rom[0x0] = 0x43; // MOV B, E
	cpu.getState().E = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.B = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_B_HTest)
{
	rom[0x0] = 0x44; // MOV B, H
	cpu.getState().H = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.B = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_B_LTest)
{
	rom[0x0] = 0x45; // MOV B, L
	cpu.getState().L = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.B = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_B_MTest)
{
	rom[0x0] = 0x46; // MOV B, M
	ram[0] = 0x42;
	cpu.getState().HL = 0x8000;

	auto preExecutionState = captureCPUState();
	u8 cycles = 7;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.B = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_B_ATest)
{
	rom[0x0] = 0x47; // MOV B, A
	cpu.getState().A = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.B = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_C_BTest)
{
	rom[0x0] = 0x48; // MOV C, B
	cpu.getState().B = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.C = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_C_CTest)
{
	rom[0x0] = 0x49; // MOV C, C

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_C_DTest)
{
	rom[0x0] = 0x4A; // MOV C, D
	cpu.getState().D = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.C = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_C_ETest)
{
	rom[0x0] = 0x4B; // MOV C, E
	cpu.getState().E = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.C = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_C_HTest)
{
	rom[0x0] = 0x4C; // MOV C, H
	cpu.getState().H = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.C = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_C_LTest)
{
	rom[0x0] = 0x4D; // MOV C, L
	cpu.getState().L = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.C = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_C_MTest)
{
	rom[0x0] = 0x4E; // MOV C, M
	ram[0] = 0x42;
	cpu.getState().HL = 0x8000;

	auto preExecutionState = captureCPUState();
	u8 cycles = 7;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.C = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_C_ATest)
{
	rom[0x0] = 0x4F; // MOV C, A
	cpu.getState().A = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.C = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_D_BTest)
{
	rom[0x0] = 0x50; // MOV D, B
	cpu.getState().B = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.D = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_D_CTest)
{
	rom[0x0] = 0x51; // MOV D, C
	cpu.getState().C = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.D = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_D_DTest)
{
	rom[0x0] = 0x52; // MOV D, D

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_D_ETest)
{
	rom[0x0] = 0x53; // MOV D, E
	cpu.getState().E = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.D = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_D_HTest)
{
	rom[0x0] = 0x54; // MOV D, H
	cpu.getState().H = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.D = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_D_LTest)
{
	rom[0x0] = 0x55; // MOV D, L
	cpu.getState().L = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.D = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_D_MTest)
{
	rom[0x0] = 0x56; // MOV D, M
	ram[0] = 0x42;
	cpu.getState().HL = 0x8000;

	auto preExecutionState = captureCPUState();
	u8 cycles = 7;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.D = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_D_ATest)
{
	rom[0x0] = 0x57; // MOV D, A
	cpu.getState().A = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.D = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_E_BTest)
{
	rom[0x0] = 0x58; // MOV E, B
	cpu.getState().B = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.E = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_E_CTest)
{
	rom[0x0] = 0x59; // MOV E, C
	cpu.getState().C = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.E = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_E_DTest)
{
	rom[0x0] = 0x5A; // MOV E, D
	cpu.getState().D = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.E = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_E_ETest)
{
	rom[0x0] = 0x5B; // MOV E, E

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_E_HTest)
{
	rom[0x0] = 0x5C; // MOV E, H
	cpu.getState().H = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.E = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_E_LTest)
{
	rom[0x0] = 0x5D; // MOV E, L
	cpu.getState().L = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.E = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_E_MTest)
{
	rom[0x0] = 0x5E; // MOV E, M
	ram[0] = 0x42;
	cpu.getState().HL = 0x8000;

	auto preExecutionState = captureCPUState();
	u8 cycles = 7;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.E = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_E_ATest)
{
	rom[0x0] = 0x5F; // MOV E, A
	cpu.getState().A = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.E = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_H_BTest)
{
	rom[0x0] = 0x60; // MOV H, B
	cpu.getState().B = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.H = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_H_CTest)
{
	rom[0x0] = 0x61; // MOV H, C
	cpu.getState().C = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.H = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_H_DTest)
{
	rom[0x0] = 0x62; // MOV H, D
	cpu.getState().D = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.H = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_H_ETest)
{
	rom[0x0] = 0x63; // MOV H, E
	cpu.getState().E = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.H = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_H_HTest)
{
	rom[0x0] = 0x64; // MOV H, H

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_H_LTest)
{
	rom[0x0] = 0x65; // MOV H, L
	cpu.getState().L = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.H = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_H_MTest)
{
	rom[0x0] = 0x66; // MOV H, M
	ram[0] = 0x42;
	cpu.getState().HL = 0x8000;

	auto preExecutionState = captureCPUState();
	u8 cycles = 7;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.H = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_H_ATest)
{
	rom[0x0] = 0x67; // MOV H, A
	cpu.getState().A = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.H = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_L_BTest)
{
	rom[0x0] = 0x68; // MOV L, B
	cpu.getState().B = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.L = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_L_CTest)
{
	rom[0x0] = 0x69; // MOV L, C
	cpu.getState().C = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.L = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_L_DTest)
{
	rom[0x0] = 0x6A; // MOV L, D
	cpu.getState().D = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.L = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_L_ETest)
{
	rom[0x0] = 0x6B; // MOV L, E
	cpu.getState().E = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.L = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_L_HTest)
{
	rom[0x0] = 0x6C; // MOV L, H
	cpu.getState().H = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.L = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_L_LTest)
{
	rom[0x0] = 0x6D; // MOV L, L

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_L_MTest)
{
	rom[0x0] = 0x6E; // MOV L, M
	ram[0] = 0x42;
	cpu.getState().HL = 0x8000;

	auto preExecutionState = captureCPUState();
	u8 cycles = 7;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.L = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_L_ATest)
{
	rom[0x0] = 0x6F; // MOV L, A
	cpu.getState().A = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.L = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_M_BTest)
{
	rom[0x0] = 0x70; // MOV M, B
	cpu.getState().B = 0x42;
	cpu.getState().HL = 0x8000;

	auto preExecutionState = captureCPUState();
	u8 cycles = 7;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(ram[0], 0x42);
}

TEST_F(CPU8080TransferTests, MOV_M_CTest)
{
	rom[0x0] = 0x71; // MOV M, C
	cpu.getState().C = 0x42;
	cpu.getState().HL = 0x8000;

	auto preExecutionState = captureCPUState();
	u8 cycles = 7;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(ram[0], 0x42);
}

TEST_F(CPU8080TransferTests, MOV_M_DTest)
{
	rom[0x0] = 0x72; // MOV M, D
	cpu.getState().D = 0x42;
	cpu.getState().HL = 0x8000;

	auto preExecutionState = captureCPUState();
	u8 cycles = 7;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(ram[0], 0x42);
}

TEST_F(CPU8080TransferTests, MOV_M_ETest)
{
	rom[0x0] = 0x73; // MOV M, E
	cpu.getState().E = 0x42;
	cpu.getState().HL = 0x8000;

	auto preExecutionState = captureCPUState();
	u8 cycles = 7;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(ram[0], 0x42);
}

TEST_F(CPU8080TransferTests, MOV_M_HTest)
{
	rom[0x0] = 0x74; // MOV M, H
	cpu.getState().HL = 0x8000;

	auto preExecutionState = captureCPUState();
	u8 cycles = 7;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(ram[0], 0x80);
}

TEST_F(CPU8080TransferTests, MOV_M_LTest)
{
	rom[0x0] = 0x75; // MOV M, L
	cpu.getState().HL = 0x8002;

	auto preExecutionState = captureCPUState();
	u8 cycles = 7;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(ram[0x2], 0x02);
}

TEST_F(CPU8080TransferTests, MOV_M_ATest)
{
	rom[0x0] = 0x77; // MOV M, A
	cpu.getState().A = 0x42;
	cpu.getState().HL = 0x8000;

	auto preExecutionState = captureCPUState();
	u8 cycles = 7;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(ram[0], 0x42);
}

TEST_F(CPU8080TransferTests, MOV_A_BTest)
{
	rom[0x0] = 0x78; // MOV A, B
	cpu.getState().B = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_A_CTest)
{
	rom[0x0] = 0x79; // MOV A, C
	cpu.getState().C = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_A_DTest)
{
	rom[0x0] = 0x7A; // MOV A, D
	cpu.getState().D = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_A_ETest)
{
	rom[0x0] = 0x7B; // MOV A, E
	cpu.getState().E = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_A_HTest)
{
	rom[0x0] = 0x7C; // MOV A, H
	cpu.getState().H = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_A_LTest)
{
	rom[0x0] = 0x7D; // MOV A, L
	cpu.getState().L = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_A_MTest)
{
	rom[0x0] = 0x7E; // MOV A, M
	ram[0] = 0x42;
	cpu.getState().HL = 0x8000;

	auto preExecutionState = captureCPUState();
	u8 cycles = 7;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MOV_A_ATest)
{
	rom[0x0] = 0x7F; // MOV A, A

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, MVITest)
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

TEST_F(CPU8080TransferTests, LXITest)
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

TEST_F(CPU8080TransferTests, LDATest)
{
	rom[0x0] = 0x3A;
	rom[0x1] = 0x00;
	rom[0x2] = 0x80; // LDA 0x8000
	ram[0x0] = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 13;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0003;
	preExecutionState.A = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, STATest)
{
	rom[0x0] = 0x32;
	rom[0x1] = 0x00;
	rom[0x2] = 0x80; // STA 0x8000
	cpu.getState().A = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 13;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0003;
	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(ram[0], 0x42);
}

TEST_F(CPU8080TransferTests, LHLDTest)
{
	rom[0x0] = 0x2A;
	rom[0x1] = 0x00;
	rom[0x2] = 0x80; // LHLD 0x8000
	ram[0x0] = 0xAD;
	ram[0x1] = 0xDE;

	auto preExecutionState = captureCPUState();
	u8 cycles = 16;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0003;
	preExecutionState.HL = 0xDEAD;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU8080TransferTests, SHLDTest)
{
	rom[0x0] = 0x22;
	rom[0x1] = 0x00;
	rom[0x2] = 0x80; // SHLD 0x8000
	cpu.getState().HL = 0xDEAD;

	auto preExecutionState = captureCPUState();
	u8 cycles = 16;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0003;
	preExecutionState.HL = 0xDEAD;
	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(ram[0], 0xAD);
	EXPECT_EQ(ram[1], 0xDE);
}
