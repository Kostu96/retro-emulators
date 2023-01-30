#include "cpu8080.hpp"

#include <gtest/gtest.h>

struct CPU8080InstructionsTests :
	public testing::Test
{
	struct CPUState
	{
		u16 PC;
		u16 SP;
		u16 AF;
		u16 BC;
		u16 DE;
		u16 HL;
	};

	struct {
		u8 buffer[2];

		u8 read(u16 address) const { return buffer[address]; }
	} ROM;

	CPU8080 CPU;
	CPUState referenceCPUState;

	CPU8080InstructionsTests() :
		CPU{ CPU8080::Mode::Intel8080 }
	{
		CPU.map(ROM, { 0x0000, 0x0001 });
	}

	void SetUp() override
	{
		CPU.reset();

		referenceCPUState.PC = CPU.getPC();
		referenceCPUState.SP = CPU.getSP();
		referenceCPUState.AF = CPU.getAF();
		referenceCPUState.BC = CPU.getBC();
		referenceCPUState.DE = CPU.getDE();
		referenceCPUState.HL = CPU.getHL();
	}

	void TearDown() override
	{
		EXPECT_EQ(referenceCPUState.PC, CPU.getPC());
		EXPECT_EQ(referenceCPUState.SP, CPU.getSP());
		EXPECT_EQ(referenceCPUState.AF, CPU.getAF());
		EXPECT_EQ(referenceCPUState.BC, CPU.getBC());
		EXPECT_EQ(referenceCPUState.DE, CPU.getDE());
		EXPECT_EQ(referenceCPUState.HL, CPU.getHL());
	}
};

TEST_F(CPU8080InstructionsTests, NOPTest)
{
	ROM.buffer[0] = 0x00;

	CPU.clock();

	referenceCPUState.PC = 0x0001;
}
