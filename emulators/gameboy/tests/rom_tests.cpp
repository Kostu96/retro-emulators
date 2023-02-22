#include "../gameboy.hpp"

#include <gtest/gtest.h>

struct BlarggCPUInstrsTests :
	public testing::TestWithParam<const char*>
{
	Gameboy gb;

	void SetUp() override
	{
		std::string testPath{ "test_files/gameboy/blargg/" };
		gb.loadCartridge((testPath + GetParam() + ".gb").c_str(), true);

		gb.reset();
	}
};

TEST_P(BlarggCPUInstrsTests, givenTestROMExpectRunSuccess)
{
	gb.runUntilEndlessLoop();

	std::string expected{ GetParam() };
	expected += "\n\n\nPassed\n";

	EXPECT_STREQ(expected.c_str(), gb.getSerialBuffer());
}

INSTANTIATE_TEST_SUITE_P(Param, BlarggCPUInstrsTests,
	testing::Values(
		"01-special",
		"02-interrupts",
		"03-op sp,hl",
		"04-op r,imm",
		"05-op rp",
		"06-ld r,r",
		"07-jr,jp,call,ret,rst",
		"08-misc instrs",
		"09-op r,r",
		"10-bit ops",
		"11-op a,(hl)"
	)
);

struct MooneyeTests :
	public testing::TestWithParam<const char*>
{
	Gameboy gb;

	void SetUp() override
	{
		std::string testPath{ "test_files/gameboy/mooneye/" };
		gb.loadCartridge((testPath + GetParam() + ".gb").c_str(), true);

		gb.reset();
	}
};

TEST_P(MooneyeTests, givenTestROMExpectRunSuccess)
{
	gb.runUntilDebugBreak();

	EXPECT_EQ(gb.m_CPU.getBC(), 0x0305); // 3 5
	EXPECT_EQ(gb.m_CPU.getDE(), 0x080D); // 8 13
	EXPECT_EQ(gb.m_CPU.getHL(), 0x1522); // 21 34
}

INSTANTIATE_TEST_SUITE_P(Param, MooneyeTests,
	testing::Values(
		"boot/boot_div-dmgABCmgb",
		"boot/boot_regs-dmgABC",
		"dma/basic",
		"dma/oam_dma_restart",
		//"dma/oam_dma_start",
		"dma/oam_dma_timing",
		"dma/reg_read",
		"timer/div_write",
		"timer/rapid_toggle",
		"timer/tim00",
		"timer/tim00_div_trigger",
		"timer/tim01",
		"timer/tim01_div_trigger",
		"timer/tim10",
		"timer/tim10_div_trigger",
		"timer/tim11",
		"timer/tim11_div_trigger",
		"timer/tima_reload",
		"timing/div_timing",
		"timing/ei_timing",
		"daa",
		"ei_sequence",
		"mem_oam",
		"reg_f",
		"unused_hwio-GS"
	)
);
