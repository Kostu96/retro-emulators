#include "../gameboy.hpp"

#include <gtest/gtest.h>

struct BlarggCPUInstrsTestParam
{
	const char* name;
	u32 updateCounter;
};

struct BlarggCPUInstrsTests :
	public testing::TestWithParam<BlarggCPUInstrsTestParam>
{
	Gameboy gb;

	void SetUp() override
	{
		gb.reset();

		std::string testPath{ "test_files/gameboy/" };
		gb.loadCartridge((testPath + GetParam().name + ".gb").c_str());
	}
};

TEST_P(BlarggCPUInstrsTests, qwerty)
{
	u32 updateCounter = GetParam().updateCounter;
	while (updateCounter--)
		gb.update();

	std::string expected{ GetParam().name };
	expected += "\n\n\nPassed\n";

	EXPECT_STREQ(expected.c_str(), gb.getSerialBuffer());
}

INSTANTIATE_TEST_SUITE_P(Param, BlarggCPUInstrsTests,
	testing::Values(
		BlarggCPUInstrsTestParam{ "01-special",             3000000 },
		BlarggCPUInstrsTestParam{ "02-interrupts",           500000 },
		BlarggCPUInstrsTestParam{ "03-op sp,hl",            4000000 },
		BlarggCPUInstrsTestParam{ "04-op r,imm",            3000000 },
		BlarggCPUInstrsTestParam{ "05-op rp",               4000000 },
		BlarggCPUInstrsTestParam{ "06-ld r,r",               600000 },
		BlarggCPUInstrsTestParam{ "07-jr,jp,call,ret,rst",   800000 },
		BlarggCPUInstrsTestParam{ "08-misc instrs",          600000 },
		BlarggCPUInstrsTestParam{ "09-op r,r",              9700000 },
		BlarggCPUInstrsTestParam{ "10-bit ops",            14700000 },
		BlarggCPUInstrsTestParam{ "11-op a,(hl)",          18600000 }
	)
);
