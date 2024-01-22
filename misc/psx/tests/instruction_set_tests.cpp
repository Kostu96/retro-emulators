#include "../source/cpu.h"
#include "../source/instruction_set.h"

#include <gtest/gtest.h>

struct CPUFixture : public testing::Test
{
    void SetUp() override
    {
        cpu.reset();
    }

    CPU cpu;
};

TEST_F(CPUFixture, tempOpCodeTester)
{
    Instruction inst = 0x81e00000;
    InstructionSet::Fns[inst.opcode()](cpu, inst);
}

TEST_F(CPUFixture, SSLtest)
{
    // ssl $zero $zero # a.k.a. nop
    Instruction inst = 0x00000000;
    InstructionSet::Fns[inst.opcode()](cpu, inst);
    // TODO: Finish
}

TEST_F(CPUFixture, ORItest)
{
    // ori $8, $8, 0x243f
    Instruction inst = 0x3508243F;
    InstructionSet::Fns[inst.opcode()](cpu, inst);
    EXPECT_EQ(cpu.getReg(8), 0xDEADBEEF | 0x243F);
}

TEST_F(CPUFixture, LUItest)
{
    // lui $8, 0x13
    Instruction inst = 0x3C080013;
    InstructionSet::Fns[inst.opcode()](cpu, inst);
    EXPECT_EQ(cpu.getReg(8), 0x13 << 16);
}
