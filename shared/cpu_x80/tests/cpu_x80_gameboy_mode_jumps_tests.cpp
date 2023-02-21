#include "cpu_x80_gameboy_mode_fixture.hpp"

using CPUx80GameBoyModeJumpsTests = CPUx80GameBoyModeTests;

TEST_F(CPUx80GameBoyModeJumpsTests, JR_Imm8Test)
{
	rom[0x0] = 0x18;
	rom[0x1] = 0x42; // JR 0x42 (66)
	rom[0x2] = 0x18;
	rom[0x3] = 0xFD; // JR 0xFD (-3)

	auto preExecutionState = captureCPUState();
	u8 cycles = 3;
	while (cycles--)
		CPU.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0044;
	compareCPUStates(preExecutionState, postExecutionState);

	CPU.m_state.PC = 0x0002;

	preExecutionState = captureCPUState();
	cycles = 3;
	while (cycles--)
		CPU.clock();
	postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeJumpsTests, RETTest)
{
	rom[0x0] = 0xC9; // RET

	ram[0x0] = 0xAD;
	ram[0x1] = 0xDE;

	CPU.m_state.SP = 0x8000;

	auto preExecutionState = captureCPUState();
	u8 cycles = 4;
	while (cycles--)
		CPU.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0xDEAD;
	preExecutionState.SP = 0x8002;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeJumpsTests, JPTest)
{
	rom[0x0] = 0xC3;
	rom[0x1] = 0xAD;
	rom[0x2] = 0xDE; // JP 0xDEAD

	auto preExecutionState = captureCPUState();
	u8 cycles = 4;
	while (cycles--)
		CPU.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0xDEAD;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeJumpsTests, JP_HLTest)
{
	rom[0x0] = 0xE9; // JP (HL)

	CPU.m_state.HL = 0xDEAD;

	auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		CPU.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0xDEAD;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeJumpsTests, RETITest)
{
	rom[0x0] = 0xD9; // RETI

	ram[0x0] = 0xAD;
	ram[0x1] = 0xDE;

	CPU.m_state.SP = 0x8000;

	auto preExecutionState = captureCPUState();
	u8 cycles = 4;
	while (cycles--)
		CPU.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0xDEAD;
	preExecutionState.SP = 0x8002;
	preExecutionState.InterruptEnabled = true;
	compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPUx80GameBoyModeJumpsTests, CALLTest)
{
	rom[0x0] = 0xCD;
	rom[0x1] = 0xAD;
	rom[0x2] = 0xDE; // CALL 0xDEAD

	CPU.m_state.SP = 0x8002;

	auto preExecutionState = captureCPUState();
	u8 cycles = 6;
	while (cycles--)
		CPU.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0xDEAD;
	preExecutionState.SP = 0x8000;
	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(ram[0], 0x03);
	EXPECT_EQ(ram[1], 0x00);
}

struct TestParam
{
	enum class Condition {
		NotZero,
		Zero,
		NotCarry,
		Carry
	};

	u8 opcode;
	Condition con;
};

struct CPUx80GameBoyModeJumpRelConTests :
	public CPUx80GameBoyModeTests,
	public testing::WithParamInterface<TestParam>
{};

TEST_P(CPUx80GameBoyModeJumpRelConTests, JR_CON_Imm8Test)
{
	rom[0x0] = GetParam().opcode;
	rom[0x1] = 0x42; // JR NZ 0x42 (66)
	rom[0x2] = GetParam().opcode;
	rom[0x3] = 0xFC; // JR NZ 0xFC (-4)

	// Condition false
	switch (GetParam().con)
	{
	case TestParam::Condition::NotZero:  CPU.m_state.F.gb.Zero  = 1; break;
	case TestParam::Condition::Zero:     CPU.m_state.F.gb.Zero  = 0; break;
	case TestParam::Condition::NotCarry: CPU.m_state.F.gb.Carry = 1; break;
	case TestParam::Condition::Carry:    CPU.m_state.F.gb.Carry = 0; break;
	}

	auto preExecutionState = captureCPUState();
	u8 cycles = 2;
	while (cycles--)
		CPU.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0002;
	compareCPUStates(preExecutionState, postExecutionState);

	// Condition true
	switch (GetParam().con)
	{
	case TestParam::Condition::NotZero:  CPU.m_state.F.gb.Zero  = 0; break;
	case TestParam::Condition::Zero:     CPU.m_state.F.gb.Zero  = 1; break;
	case TestParam::Condition::NotCarry: CPU.m_state.F.gb.Carry = 0; break;
	case TestParam::Condition::Carry:    CPU.m_state.F.gb.Carry = 1; break;
	}

	preExecutionState = captureCPUState();
	cycles = 3;
	while (cycles--)
		CPU.clock();
	postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0000;
	compareCPUStates(preExecutionState, postExecutionState);

	preExecutionState = captureCPUState();
	cycles = 3;
	while (cycles--)
		CPU.clock();
	postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0044;
	compareCPUStates(preExecutionState, postExecutionState);
}

INSTANTIATE_TEST_SUITE_P(Param, CPUx80GameBoyModeJumpRelConTests,
	testing::Values(
		TestParam{ 0x20, TestParam::Condition::NotZero },
		TestParam{ 0x28, TestParam::Condition::Zero },
		TestParam{ 0x30, TestParam::Condition::NotCarry },
		TestParam{ 0x38, TestParam::Condition::Carry }
	)
);

struct CPUx80GameBoyModeRetConTests :
	public CPUx80GameBoyModeTests,
	public testing::WithParamInterface<TestParam>
{};

TEST_P(CPUx80GameBoyModeRetConTests, RET_CONTest)
{
	rom[0x0] = GetParam().opcode;
	rom[0x1] = GetParam().opcode;

	ram[0x0] = 0xAD;
	ram[0x1] = 0xDE;

	CPU.m_state.SP = 0x8000;

	// Condition false
	switch (GetParam().con)
	{
	case TestParam::Condition::NotZero:  CPU.m_state.F.gb.Zero = 1; break;
	case TestParam::Condition::Zero:     CPU.m_state.F.gb.Zero = 0; break;
	case TestParam::Condition::NotCarry: CPU.m_state.F.gb.Carry = 1; break;
	case TestParam::Condition::Carry:    CPU.m_state.F.gb.Carry = 0; break;
	}

	auto preExecutionState = captureCPUState();
	u8 cycles = 2;
	while (cycles--)
		CPU.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);

	// Condition true
	switch (GetParam().con)
	{
	case TestParam::Condition::NotZero:  CPU.m_state.F.gb.Zero = 0; break;
	case TestParam::Condition::Zero:     CPU.m_state.F.gb.Zero = 1; break;
	case TestParam::Condition::NotCarry: CPU.m_state.F.gb.Carry = 0; break;
	case TestParam::Condition::Carry:    CPU.m_state.F.gb.Carry = 1; break;
	}

	preExecutionState = captureCPUState();
	cycles = 4;
	while (cycles--)
		CPU.clock();
	postExecutionState = captureCPUState();

	preExecutionState.PC = 0xDEAD;
	preExecutionState.SP = 0x8002;
	compareCPUStates(preExecutionState, postExecutionState);
}

INSTANTIATE_TEST_SUITE_P(Param, CPUx80GameBoyModeRetConTests,
	testing::Values(
		TestParam{ 0xC0, TestParam::Condition::NotZero },
		TestParam{ 0xC8, TestParam::Condition::Zero },
		TestParam{ 0xD0, TestParam::Condition::NotCarry },
		TestParam{ 0xD8, TestParam::Condition::Carry }
	)
);

struct CPUx80GameBoyModeJumpConTests :
	public CPUx80GameBoyModeTests,
	public testing::WithParamInterface<TestParam>
{};

TEST_P(CPUx80GameBoyModeJumpConTests, JP_CONTest)
{
	rom[0x0] = GetParam().opcode;
	rom[0x1] = 0xAD;
	rom[0x2] = 0xDE;

	// Condition false
	switch (GetParam().con)
	{
	case TestParam::Condition::NotZero:  CPU.m_state.F.gb.Zero = 1; break;
	case TestParam::Condition::Zero:     CPU.m_state.F.gb.Zero = 0; break;
	case TestParam::Condition::NotCarry: CPU.m_state.F.gb.Carry = 1; break;
	case TestParam::Condition::Carry:    CPU.m_state.F.gb.Carry = 0; break;
	}

	auto preExecutionState = captureCPUState();
	u8 cycles = 3;
	while (cycles--)
		CPU.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0003;
	compareCPUStates(preExecutionState, postExecutionState);

	// Condition true
	switch (GetParam().con)
	{
	case TestParam::Condition::NotZero:  CPU.m_state.F.gb.Zero = 0; break;
	case TestParam::Condition::Zero:     CPU.m_state.F.gb.Zero = 1; break;
	case TestParam::Condition::NotCarry: CPU.m_state.F.gb.Carry = 0; break;
	case TestParam::Condition::Carry:    CPU.m_state.F.gb.Carry = 1; break;
	}

	CPU.m_state.PC = 0x0000;

	preExecutionState = captureCPUState();
	cycles = 4;
	while (cycles--)
		CPU.clock();
	postExecutionState = captureCPUState();

	preExecutionState.PC = 0xDEAD;
	compareCPUStates(preExecutionState, postExecutionState);
}

INSTANTIATE_TEST_SUITE_P(Param, CPUx80GameBoyModeJumpConTests,
	testing::Values(
		TestParam{ 0xC2, TestParam::Condition::NotZero },
		TestParam{ 0xCA, TestParam::Condition::Zero },
		TestParam{ 0xD2, TestParam::Condition::NotCarry },
		TestParam{ 0xDA, TestParam::Condition::Carry }
	)
);

struct CPUx80GameBoyModeCallConTests :
	public CPUx80GameBoyModeTests,
	public testing::WithParamInterface<TestParam>
{};

TEST_P(CPUx80GameBoyModeCallConTests, CALL_CONTest)
{
	rom[0x0] = GetParam().opcode;
	rom[0x1] = 0xAD;
	rom[0x2] = 0xDE;

	CPU.m_state.SP = 0x8002;

	// Condition false
	switch (GetParam().con)
	{
	case TestParam::Condition::NotZero:  CPU.m_state.F.gb.Zero = 1; break;
	case TestParam::Condition::Zero:     CPU.m_state.F.gb.Zero = 0; break;
	case TestParam::Condition::NotCarry: CPU.m_state.F.gb.Carry = 1; break;
	case TestParam::Condition::Carry:    CPU.m_state.F.gb.Carry = 0; break;
	}

	auto preExecutionState = captureCPUState();
	u8 cycles = 3;
	while (cycles--)
		CPU.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0003;
	compareCPUStates(preExecutionState, postExecutionState);

	// Condition true
	switch (GetParam().con)
	{
	case TestParam::Condition::NotZero:  CPU.m_state.F.gb.Zero = 0; break;
	case TestParam::Condition::Zero:     CPU.m_state.F.gb.Zero = 1; break;
	case TestParam::Condition::NotCarry: CPU.m_state.F.gb.Carry = 0; break;
	case TestParam::Condition::Carry:    CPU.m_state.F.gb.Carry = 1; break;
	}

	CPU.m_state.PC = 0x0000;

	preExecutionState = captureCPUState();
	cycles = 6;
	while (cycles--)
		CPU.clock();
	postExecutionState = captureCPUState();

	preExecutionState.PC = 0xDEAD;
	preExecutionState.SP = 0x8000;
	compareCPUStates(preExecutionState, postExecutionState);
	EXPECT_EQ(ram[0], 0x03);
	EXPECT_EQ(ram[1], 0x00);
}

INSTANTIATE_TEST_SUITE_P(Param, CPUx80GameBoyModeCallConTests,
	testing::Values(
		TestParam{ 0xC4, TestParam::Condition::NotZero },
		TestParam{ 0xCC, TestParam::Condition::Zero },
		TestParam{ 0xD4, TestParam::Condition::NotCarry },
		TestParam{ 0xDC, TestParam::Condition::Carry }
	)
);
