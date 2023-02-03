#include "cpu8080.hpp"

#include <gtest/gtest.h>

struct CPU8080ModeGameBoyTests :
	public testing::Test
{
	static constexpr u16 ROM_SIZE = 0x10;
	static constexpr u16 RAM_SIZE = 0x10;

	u8 rom[ROM_SIZE]{};
	u8 ram[RAM_SIZE]{};

	CPU8080 CPU;
	CPU8080::State referenceCPUState;

	CPU8080ModeGameBoyTests() :
		CPU{ CPU8080::Mode::GameBoy },
		referenceCPUState{ CPU8080::Mode::GameBoy }
	{
		CPU.mapReadMemoryCallback([&](u16 address)
			{
				if (address < ROM_SIZE) return rom[address];
				if (address >= 0x8000) return ram[address - 0x8000];
				return u8{};
			});

		CPU.mapWriteMemoryCallback([&](u16 address, u8 data)
			{
				if (address >= 0x8000) ram[address - 0x8000] = data;
			});
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

TEST_F(CPU8080ModeGameBoyTests, NOPTest)
{
	rom[0] = 0x00; // NOP

	CPU.clock();

	referenceCPUState.PC = 0x0001;
}
