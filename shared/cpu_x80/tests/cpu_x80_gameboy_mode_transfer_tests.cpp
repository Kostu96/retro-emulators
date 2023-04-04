#include "cpu_x80_gameboy_mode_fixture.hpp"

using CPUx80GameBoyModeTransferTests = CPUx80GameBoyModeTests;

TEST_F(CPUx80GameBoyModeTransferTests, LD_RP_Imm16Test)
{
	rom[0x0] = 0x01;
	rom[0x1] = 0x23;
	rom[0x2] = 0x01; // LD BC, 0x0123
	rom[0x3] = 0x11;
	rom[0x4] = 0x67;
	rom[0x5] = 0x45; // LD DE, 0x4567
	rom[0x6] = 0x21;
	rom[0x7] = 0xAB;
	rom[0x8] = 0x89; // LD HL, 0x89AB
	rom[0x9] = 0x31;
	rom[0xA] = 0xEF;
	rom[0xB] = 0xCD; // LD SP, 0xCDEF

	auto preExecutionState = captureCPUState();
	u8 cycles = 4 * 3;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.BC = 0x0123;
	preExecutionState.DE = 0x4567;
	preExecutionState.HL = 0x89AB;
	preExecutionState.SP = 0xCDEF;
	preExecutionState.PC = 0x000C;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_RP_ATest)
{
	rom[0x0] = 0x02; // LD (BC), A
	rom[0x1] = 0x12; // LD (DE), A
	rom[0x2] = 0x22; // LD (HL+), A
	rom[0x3] = 0x32; // LD (HL-), A

	cpu.m_state.BC = 0x8000;
	cpu.m_state.DE = 0x8001;
	cpu.m_state.HL = 0x8002;
	cpu.m_state.A = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 4 * 2;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0004;
	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(ram[0], 0x42);
	EXPECT_EQ(ram[1], 0x42);
	EXPECT_EQ(ram[2], 0x42);
	EXPECT_EQ(ram[3], 0x42);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_R_Imm8Test)
{
	rom[0x0] = 0x06;
	rom[0x1] = 0x01; // LD B, 0x01
	rom[0x2] = 0x0E;
	rom[0x3] = 0x23; // LD C, 0x23
	rom[0x4] = 0x16;
	rom[0x5] = 0x45; // LD D, 0x45
	rom[0x6] = 0x1E;
	rom[0x7] = 0x67; // LD E, 0x67
	rom[0x8] = 0x26;
	rom[0x9] = 0x89; // LD H, 0x89
	rom[0xA] = 0x2E;
	rom[0xB] = 0xAB; // LD L, 0xAB
	rom[0xC] = 0x3E;
	rom[0xD] = 0xCD; // LD A, 0xCD
	rom[0xE] = 0x36;
	rom[0xF] = 0xEF; // LD (HL), 0xEF

	auto preExecutionState = captureCPUState();
	u8 cycles = 7 * 2;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x000E;
	preExecutionState.B = 0x01;
	preExecutionState.C = 0x23;
	preExecutionState.D = 0x45;
	preExecutionState.E = 0x67;
	preExecutionState.H = 0x89;
	preExecutionState.L = 0xAB;
	preExecutionState.A = 0xCD;
	compareCPUStates(preExecutionState, postExecutionState);

	cpu.m_state.HL = 0x8000;

	preExecutionState = captureCPUState();
	cycles = 3;
	while (cycles--)
		cpu.clock();
	postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0010;
	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(ram[0], 0xEF);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_Imm16_SPTest)
{
	rom[0x0] = 0x08;
	rom[0x1] = 0x16;
	rom[0x2] = 0x80; // LD 0x8016, SP

	cpu.m_state.SP = 0xDEAD;

	auto preExecutionState = captureCPUState();
	u8 cycles = 5;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0003;
	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(ram[0x16], 0xAD);
	EXPECT_EQ(ram[0x17], 0xDE);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_A_RPTest)
{
	rom[0x0] = 0x0A; // LD A, (BC)
	rom[0x1] = 0x1A; // LD A, (DE)
	rom[0x2] = 0x2A; // LD A, (HL+)
	rom[0x3] = 0x3A; // LD A, (HL-)

	cpu.m_state.BC = 0x8000;
	cpu.m_state.DE = 0x8001;
	cpu.m_state.HL = 0x8002;

	ram[0] = 0x01;
	ram[1] = 0x23;
	ram[2] = 0x45;
	ram[3] = 0x67;

	auto preExecutionState = captureCPUState();
	u8 cycles = 2;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x01;
	compareCPUStates(preExecutionState, postExecutionState);

	preExecutionState = captureCPUState();
	cycles = 2;
	while (cycles--)
		cpu.clock();
	postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0002;
	preExecutionState.A = 0x23;
	compareCPUStates(preExecutionState, postExecutionState);

	preExecutionState = captureCPUState();
	cycles = 2;
	while (cycles--)
		cpu.clock();
	postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0003;
	preExecutionState.A = 0x45;
	preExecutionState.HL = 0x8003;
	compareCPUStates(preExecutionState, postExecutionState);

	preExecutionState = captureCPUState();
	cycles = 2;
	while (cycles--)
		cpu.clock();
	postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0004;
	preExecutionState.A = 0x67;
	preExecutionState.HL = 0x8002;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_B_BTest)
{
	rom[0x0] = 0x40; // LD B, B

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_B_CTest)
{
	rom[0x0] = 0x41; // LD B, C
	cpu.m_state.C = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.B = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_B_DTest)
{
	rom[0x0] = 0x42; // LD B, D
	cpu.m_state.D = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.B = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_B_ETest)
{
	rom[0x0] = 0x43; // LD B, E
	cpu.m_state.E = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.B = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_B_HTest)
{
	rom[0x0] = 0x44; // LD B, H
	cpu.m_state.H = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.B = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_B_LTest)
{
	rom[0x0] = 0x45; // LD B, L
	cpu.m_state.L = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.B = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_B_HLTest)
{
	rom[0x0] = 0x46; // LD B, (HL)
	ram[0] = 0x42;
	cpu.m_state.HL = 0x8000;

	auto preExecutionState = captureCPUState();
	u8 cycles = 2;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.B = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_B_ATest)
{
	rom[0x0] = 0x47; // LD B, A
	cpu.m_state.A = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.B = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_C_BTest)
{
	rom[0x0] = 0x48; // LD C, B
	cpu.m_state.B = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.C = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_C_CTest)
{
	rom[0x0] = 0x49; // LD C, C

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_C_DTest)
{
	rom[0x0] = 0x4A; // LD C, D
	cpu.m_state.D = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.C = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_C_ETest)
{
	rom[0x0] = 0x4B; // LD C, E
	cpu.m_state.E = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.C = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_C_HTest)
{
	rom[0x0] = 0x4C; // LD C, H
	cpu.m_state.H = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.C = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_C_LTest)
{
	rom[0x0] = 0x4D; // LD C, L
	cpu.m_state.L = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.C = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_C_HLTest)
{
	rom[0x0] = 0x4E; // LD C, (HL)
	ram[0] = 0x42;
	cpu.m_state.HL = 0x8000;

	auto preExecutionState = captureCPUState();
	u8 cycles = 2;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.C = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_C_ATest)
{
	rom[0x0] = 0x4F; // LD C, A
	cpu.m_state.A = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.C = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_D_BTest)
{
	rom[0x0] = 0x50; // LD D, B
	cpu.m_state.B = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.D = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_D_CTest)
{
	rom[0x0] = 0x51; // LD D, C
	cpu.m_state.C = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.D = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_D_DTest)
{
	rom[0x0] = 0x52; // LD D, D

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_D_ETest)
{
	rom[0x0] = 0x53; // LD D, E
	cpu.m_state.E = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.D = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_D_HTest)
{
	rom[0x0] = 0x54; // LD D, H
	cpu.m_state.H = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.D = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_D_LTest)
{
	rom[0x0] = 0x55; // LD D, L
	cpu.m_state.L = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.D = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_D_HLTest)
{
	rom[0x0] = 0x56; // LD D, (HL)
	ram[0] = 0x42;
	cpu.m_state.HL = 0x8000;

	auto preExecutionState = captureCPUState();
	u8 cycles = 2;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.D = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_D_ATest)
{
	rom[0x0] = 0x57; // LD D, A
	cpu.m_state.A = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.D = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_E_BTest)
{
	rom[0x0] = 0x58; // LD E, B
	cpu.m_state.B = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.E = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_E_CTest)
{
	rom[0x0] = 0x59; // LD E, C
	cpu.m_state.C = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.E = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_E_DTest)
{
	rom[0x0] = 0x5A; // LD E, D
	cpu.m_state.D = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.E = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_E_ETest)
{
	rom[0x0] = 0x5B; // LD E, E

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_E_HTest)
{
	rom[0x0] = 0x5C; // LD E, H
	cpu.m_state.H = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.E = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_E_LTest)
{
	rom[0x0] = 0x5D; // LD E, L
	cpu.m_state.L = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.E = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_E_HLTest)
{
	rom[0x0] = 0x5E; // LD E, (HL)
	ram[0] = 0x42;
	cpu.m_state.HL = 0x8000;

	auto preExecutionState = captureCPUState();
	u8 cycles = 2;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.E = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_E_ATest)
{
	rom[0x0] = 0x5F; // LD E, A
	cpu.m_state.A = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.E = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_H_BTest)
{
	rom[0x0] = 0x60; // LD H, B
	cpu.m_state.B = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.H = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_H_CTest)
{
	rom[0x0] = 0x61; // LD H, C
	cpu.m_state.C = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.H = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_H_DTest)
{
	rom[0x0] = 0x62; // LD H, D
	cpu.m_state.D = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.H = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_H_ETest)
{
	rom[0x0] = 0x63; // LD H, E
	cpu.m_state.E = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.H = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_H_HTest)
{
	rom[0x0] = 0x64; // LD H, H

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_H_LTest)
{
	rom[0x0] = 0x65; // LD H, L
	cpu.m_state.L = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.H = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_H_HLTest)
{
	rom[0x0] = 0x66; // LD H, (HL)
	ram[0] = 0x42;
	cpu.m_state.HL = 0x8000;

	auto preExecutionState = captureCPUState();
	u8 cycles = 2;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.H = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_H_ATest)
{
	rom[0x0] = 0x67; // LD H, A
	cpu.m_state.A = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.H = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_L_BTest)
{
	rom[0x0] = 0x68; // LD L, B
	cpu.m_state.B = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.L = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_L_CTest)
{
	rom[0x0] = 0x69; // LD L, C
	cpu.m_state.C = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.L = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_L_DTest)
{
	rom[0x0] = 0x6A; // LD L, D
	cpu.m_state.D = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.L = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_L_ETest)
{
	rom[0x0] = 0x6B; // LD L, E
	cpu.m_state.E = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.L = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_L_HTest)
{
	rom[0x0] = 0x6C; // LD L, H
	cpu.m_state.H = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.L = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_L_LTest)
{
	rom[0x0] = 0x6D; // LD L, L

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_L_HLTest)
{
	rom[0x0] = 0x6E; // LD L, (HL)
	ram[0] = 0x42;
	cpu.m_state.HL = 0x8000;

	auto preExecutionState = captureCPUState();
	u8 cycles = 2;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.L = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_L_ATest)
{
	rom[0x0] = 0x6F; // LD L, A
	cpu.m_state.A = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.L = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_HL_BTest)
{
	rom[0x0] = 0x70; // LD (HL), B
	cpu.m_state.B = 0x42;
	cpu.m_state.HL = 0x8000;

	auto preExecutionState = captureCPUState();
	u8 cycles = 2;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(ram[0], 0x42);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_HL_CTest)
{
	rom[0x0] = 0x71; // LD (HL), C
	cpu.m_state.C = 0x42;
	cpu.m_state.HL = 0x8000;

	auto preExecutionState = captureCPUState();
	u8 cycles = 2;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(ram[0], 0x42);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_HL_DTest)
{
	rom[0x0] = 0x72; // LD (HL), D
	cpu.m_state.D = 0x42;
	cpu.m_state.HL = 0x8000;

	auto preExecutionState = captureCPUState();
	u8 cycles = 2;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(ram[0], 0x42);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_HL_ETest)
{
	rom[0x0] = 0x73; // LD (HL), E
	cpu.m_state.E = 0x42;
	cpu.m_state.HL = 0x8000;

	auto preExecutionState = captureCPUState();
	u8 cycles = 2;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(ram[0], 0x42);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_HL_HTest)
{
	rom[0x0] = 0x74; // LD (HL), H
	cpu.m_state.HL = 0x8000;

	auto preExecutionState = captureCPUState();
	u8 cycles = 2;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(ram[0], 0x80);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_HL_LTest)
{
	rom[0x0] = 0x75; // LD (HL), L
	cpu.m_state.HL = 0x8002;

	auto preExecutionState = captureCPUState();
	u8 cycles = 2;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(ram[0x2], 0x02);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_HL_ATest)
{
	rom[0x0] = 0x77; // LD (HL), A
	cpu.m_state.A = 0x42;
	cpu.m_state.HL = 0x8000;

	auto preExecutionState = captureCPUState();
	u8 cycles = 2;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(ram[0], 0x42);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_A_BTest)
{
	rom[0x0] = 0x78; // LD A, B
	cpu.m_state.B = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_A_CTest)
{
	rom[0x0] = 0x79; // LD A, C
	cpu.m_state.C = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_A_DTest)
{
	rom[0x0] = 0x7A; // LD A, D
	cpu.m_state.D = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_A_ETest)
{
	rom[0x0] = 0x7B; // LD A, E
	cpu.m_state.E = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_A_HTest)
{
	rom[0x0] = 0x7C; // LD A, H
	cpu.m_state.H = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_A_LTest)
{
	rom[0x0] = 0x7D; // LD A, L
	cpu.m_state.L = 0x42;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_A_HLTest)
{
	rom[0x0] = 0x7E; // LD A, (HL)
	ram[0] = 0x42;
	cpu.m_state.HL = 0x8000;

	auto preExecutionState = captureCPUState();
	u8 cycles = 2;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_A_ATest)
{
	rom[0x0] = 0x7F; // LD A, A

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, POP_RPTest)
{
	rom[0x0] = 0xC1; // POP BC
	rom[0x1] = 0xD1; // POP DE
	rom[0x2] = 0xE1; // POP HL
	rom[0x3] = 0xF1; // POP AF

	ram[0x1F] = 0xCD;
	ram[0x1E] = 0xEF;
	ram[0x1D] = 0x89;
	ram[0x1C] = 0xAB;
	ram[0x1B] = 0x45;
	ram[0x1A] = 0x67;
	ram[0x19] = 0x01;
	ram[0x18] = 0x23;

	cpu.m_state.SP = 0x8018;

	auto preExecutionState = captureCPUState();
	u8 cycles = 4 * 3;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.BC = 0x0123;
	preExecutionState.DE = 0x4567;
	preExecutionState.HL = 0x89AB;
	preExecutionState.AF = 0xCDE0;
	preExecutionState.SP = 0x8020;
	preExecutionState.PC = 0x0004;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, PUSH_RPTest)
{
	rom[0x0] = 0xC5; // PUSH BC
	rom[0x1] = 0xD5; // PUSH DE
	rom[0x2] = 0xE5; // PUSH HL
	rom[0x3] = 0xF5; // PUSH AF

	cpu.m_state.BC = 0x0123;
	cpu.m_state.DE = 0x4567;
	cpu.m_state.HL = 0x89AB;
	cpu.m_state.AF = 0xCDEF;
	cpu.m_state.SP = 0x8020;

	auto preExecutionState = captureCPUState();
	u8 cycles = 4 * 4;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.SP = 0x8018;
	preExecutionState.PC = 0x0004;
	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(ram[0x1F], 0x01);
	EXPECT_EQ(ram[0x1E], 0x23);
	EXPECT_EQ(ram[0x1D], 0x45);
	EXPECT_EQ(ram[0x1C], 0x67);
	EXPECT_EQ(ram[0x1B], 0x89);
	EXPECT_EQ(ram[0x1A], 0xAB);
	EXPECT_EQ(ram[0x19], 0xCD);
	EXPECT_EQ(ram[0x18], 0xEF);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_Imm8Indirect_ATest)
{
	rom[0x0] = 0xE0;
	rom[0x1] = 0x02; // LDH 0x02, A
	cpu.m_state.A = 0x42;

	auto preExecutionState = captureCPUState();
	cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0002;
	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(ram[0x2], 0x42);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_CIndirect_ATest)
{
	rom[0x0] = 0xE2; // LD (C), A
	cpu.m_state.C = 0x02;
	cpu.m_state.A = 0x42;

	auto preExecutionState = captureCPUState();
	cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(ram[0x2], 0x42);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_Imm16_ATest)
{
	rom[0x0] = 0xEA;
	rom[0x1] = 0x02;
	rom[0x2] = 0x80; // LD 0x8002, A
	cpu.m_state.A = 0x42;

	auto preExecutionState = captureCPUState();
	cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0003;
	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(ram[0x2], 0x42);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_A_Imm8IndirectTest)
{
	rom[0x0] = 0xF0;
	rom[0x1] = 0x02; // LD A, 0x02
	ram[0x2] = 0x42;

	auto preExecutionState = captureCPUState();
	cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0002;
	preExecutionState.A = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_A_CIndirectTest)
{
	rom[0x0] = 0xF2; // LD A, (C)
	cpu.m_state.C = 0x02;
	ram[0x2] = 0x42;

	auto preExecutionState = captureCPUState();
	cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.A = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_HL_SPPlusImm8Test)
{
	rom[0x0] = 0xF8;
	rom[0x1] = 0x42; // LD HL, SP + 0x42 (66)
	rom[0x2] = 0xF8;
	rom[0x3] = 0xBE; // LD HL, SP + 0xBE (-66)

	cpu.m_state.SP = 0xDE00;

	auto preExecutionState = captureCPUState();
	u8 cycles = 3;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0002;
	preExecutionState.HL = 0xDE42;
	compareCPUStates(preExecutionState, postExecutionState);

	cpu.m_state.SP = 0xDE42;

	preExecutionState = captureCPUState();
	cycles = 3;
	while (cycles--)
		cpu.clock();
	postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0004;
	preExecutionState.HL = 0xDE00;
	preExecutionState.F.HalfCarry = 1;
	preExecutionState.F.Carry = 1;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_SP_HLTest)
{
	rom[0x0] = 0xF9; // LD SP, HL

	cpu.m_state.HL = 0xDEAD;

	auto preExecutionState = captureCPUState();
	cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	preExecutionState.SP = 0xDEAD;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeTransferTests, LD_A_Imm16IndirectTest)
{
	rom[0x0] = 0xFA;
	rom[0x1] = 0x02;
	rom[0x2] = 0x80; // LD A, 0x8002
	ram[0x2] = 0x42;

	auto preExecutionState = captureCPUState();
	cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0003;
	preExecutionState.A = 0x42;
	compareCPUStates(preExecutionState, postExecutionState);
}
