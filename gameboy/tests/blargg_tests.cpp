#include "../gameboy.hpp"

#include <gtest/gtest.h>

struct BlarggTestParam
{
	const char* name;
	u32 updateCounter;
};

struct BlarggTests :
	public testing::TestWithParam<BlarggTestParam>
{
	Gameboy gb;

	void SetUp() override
	{
		gb.reset();

		std::string testPath{ "test_files/gameboy/" };
		gb.loadCartridge((testPath + GetParam().name + ".gb").c_str());
	}
};

TEST_P(BlarggTests, qwerty)
{
	u32 updateCounter = GetParam().updateCounter;
	while (updateCounter--)
		gb.update();

	std::string expected{ GetParam().name };
	expected += "\n\n\nPassed\n";

	EXPECT_STREQ(expected.c_str(), gb.getSerialBuffer());
}

INSTANTIATE_TEST_SUITE_P(Param, BlarggTests,
	testing::Values(
		BlarggTestParam{ "01-special",            3000000 },
		BlarggTestParam{ "02-interrupts",         500000 },
		BlarggTestParam{ "03-op sp,hl",           4000000 }/*,
		BlarggTestParam{ "04-op r,imm",           500000 },
		BlarggTestParam{ "05-op rp",              500000 },
		BlarggTestParam{ "06-ld r,r",             500000 },
		BlarggTestParam{ "07-jr,jp,call,ret,rst", 500000 },
		BlarggTestParam{ "08-misc instrs",        500000 },
		BlarggTestParam{ "09-op r,r",             500000 },
		BlarggTestParam{ "10-bit ops",            500000 },
		BlarggTestParam{ "11-op a,(hl)",          500000 }*/
	)
);
