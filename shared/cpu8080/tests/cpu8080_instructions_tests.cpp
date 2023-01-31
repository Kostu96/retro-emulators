#include "cpu8080.hpp"

#include <gtest/gtest.h>

struct CPU8080InstructionsTests :
	public testing::Test
{
	static constexpr u16 ROM_SIZE = 12;

	struct {
		u8 buffer[ROM_SIZE]{};

		u8 read(u16 address) const { return buffer[address]; }
	} ROM;

	CPU8080 CPU;
	CPU8080::State referenceCPUState;

	CPU8080InstructionsTests() :
		CPU{ CPU8080::Mode::Intel8080 }
	{
		CPU.map(ROM, { 0x0000, ROM_SIZE - 1 });
	}

	void SetUp() override
	{
		CPU.reset();

		std::memcpy(&referenceCPUState, &CPU.m_state, sizeof(CPU8080::State));
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

TEST_F(CPU8080InstructionsTests, JMPTest)
{
	ROM.buffer[0] = 0xC3;
	ROM.buffer[1] = 0xAD;
	ROM.buffer[2] = 0xDE;

	CPU.clock(); // TODO: should take 3 cycles

	referenceCPUState.PC = 0xDEAD;
}

TEST_F(CPU8080InstructionsTests, LXITest)
{
	ROM.buffer[0x0] = 0x01;
	ROM.buffer[0x1] = 0x23;
	ROM.buffer[0x2] = 0x01;

	ROM.buffer[0x3] = 0x11;
	ROM.buffer[0x4] = 0x67;
	ROM.buffer[0x5] = 0x45;

	ROM.buffer[0x6] = 0x21;
	ROM.buffer[0x7] = 0xAB;
	ROM.buffer[0x8] = 0x89;

	ROM.buffer[0x9] = 0x31;
	ROM.buffer[0xA] = 0xEF;
	ROM.buffer[0xB] = 0xCD;

	int cycles = 4; // TODO: should take 12 cycles
	while (cycles--)
		CPU.clock();

	referenceCPUState.BC = 0x0123;
	referenceCPUState.DE = 0x4567;
	referenceCPUState.HL = 0x89AB;
	referenceCPUState.SP = 0xCDEF;
	referenceCPUState.PC = 0x000C;
}
