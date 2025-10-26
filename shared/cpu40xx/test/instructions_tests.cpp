#include <cpu40xx/cpu40xx.hpp>

#include <gtest/gtest.h>
#include <cstring>

namespace {

struct CPU4004InstructionsTests :
    public ::testing::Test {
    static constexpr u16 ROM_SIZE = 0x10;

    u8 rom[ROM_SIZE]{};

    CPU4004InstructionsTests() :
        cpu(CPU40xx::Mode::Intel4004) {
        cpu.mapReadROMCallback([this](u16 address) -> u8 {
            if (address < ROM_SIZE) return rom[address];
            return 0u;
        });
    }

    void SetUp() override {
        memset(rom, 0, ROM_SIZE);
        cpu.reset();
    }

    void TearDown() override {
    
    }

    CPU40xx::State captureCPUState() const {
        return cpu.getState();
    }

    void compareCPUStates(const CPU40xx::State& state1, const CPU40xx::State& state2) {
        EXPECT_EQ(state1.regs, state2.regs);
        EXPECT_EQ(state1.stack, state2.stack);
    }

    CPU40xx cpu;
};

TEST_F(CPU4004InstructionsTests, NOPTest) {
    rom[0] = 0x0; // NOP

    auto preExecutionState = captureCPUState();
	u8 cycles = 1;
	while (cycles--)
		cpu.clock();
	auto postExecutionState = captureCPUState();

    preExecutionState.stack[0]++;
    compareCPUStates(preExecutionState, postExecutionState);
}

}
