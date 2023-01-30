#include "cpu8080.hpp"

#include <gtest/gtest.h>

struct CPU8080InstructionsTests :
	public testing::Test
{
	CPU8080 cpu;

	CPU8080InstructionsTests() :
		cpu{ CPU8080::Mode::Intel8080 } {}

	void SetUp() override
	{
		cpu.reset();
	}

	void TearDown() override
	{

	}
};

TEST_F(CPU8080InstructionsTests, NOPTest)
{

}
