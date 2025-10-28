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

        // give some different values to all registers
        u8 value = 0;
        for (u8& reg : cpu.getState().regs) {
            reg = value++;
        }
        value = 0;
        for (u16& slot : cpu.getState().stack) {
            slot = value++;
        }
    }

    void TearDown() override {}

    CPU40xx::State captureCPUState() const {
        return cpu.getState();
    }

    void compareCPUStates(const CPU40xx::State& state1, const CPU40xx::State& state2) {
        EXPECT_EQ(state1.regs, state2.regs);
        EXPECT_EQ(state1.stack, state2.stack);
        EXPECT_EQ(state1.ACC, state2.ACC);
        EXPECT_EQ(state1.SP, state2.SP);
        EXPECT_EQ(state1.CY, state2.CY);
        EXPECT_EQ(state1.test, state2.test);
        EXPECT_EQ(state1.CMRAM, state2.CMRAM);
        EXPECT_EQ(state1.SRCReg, state2.SRCReg);
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

TEST_F(CPU4004InstructionsTests, LDMTest) {
    rom[0] = 0xDA; // LDM 10

    auto preExecutionState = captureCPUState();
    u8 cycles = 1;
    while (cycles--)
        cpu.clock();
    auto postExecutionState = captureCPUState();

    preExecutionState.stack[0]++;
    preExecutionState.ACC = 10;
    compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU4004InstructionsTests, LDTest) {
    rom[0] = 0xAC; // LD 12

    auto preExecutionState = captureCPUState();
    u8 cycles = 1;
    while (cycles--)
        cpu.clock();
    auto postExecutionState = captureCPUState();

    preExecutionState.stack[0]++;
    preExecutionState.ACC = 12;
    compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU4004InstructionsTests, XCHTest) {
    rom[0] = 0xBC; // XCH 12
    cpu.getState().ACC = 4;
    
    u8 value = 0;
    for (u8& reg : cpu.getState().regs) {
        reg = value++;
    }

    auto preExecutionState = captureCPUState();
    u8 cycles = 1;
    while (cycles--)
        cpu.clock();
    auto postExecutionState = captureCPUState();

    preExecutionState.stack[0]++;
    preExecutionState.ACC = 12;
    preExecutionState.regs[12] = 4;
    compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU4004InstructionsTests, ADDTest) {
    rom[0] = 0x8C; // ADD 12 (no carry)
    rom[1] = 0x8D; // ADD 13 (produces carry)
    rom[2] = 0x8E; // ADD 14 (consumes and produces carry)
    rom[3] = 0x8F; // ADD 15 (consumes carry)
    cpu.getState().ACC = 4;
    cpu.getState().regs[12] = 5;
    cpu.getState().regs[13] = 8;
    cpu.getState().regs[14] = 14;
    cpu.getState().regs[15] = 2;

    auto preExecutionState = captureCPUState();
    u8 cycles = 1;
    while (cycles--)
        cpu.clock();
    auto postExecutionState = captureCPUState();

    preExecutionState.stack[0]++;
    preExecutionState.ACC = 9;
    compareCPUStates(preExecutionState, postExecutionState);

    preExecutionState = postExecutionState;
    cycles = 1;
    while (cycles--)
        cpu.clock();
    postExecutionState = captureCPUState();

    preExecutionState.stack[0]++;
    preExecutionState.ACC = 1;
    preExecutionState.CY = 1;
    compareCPUStates(preExecutionState, postExecutionState);

    preExecutionState = postExecutionState;
    cycles = 1;
    while (cycles--)
        cpu.clock();
    postExecutionState = captureCPUState();

    preExecutionState.stack[0]++;
    preExecutionState.ACC = 0;
    compareCPUStates(preExecutionState, postExecutionState);

    preExecutionState = postExecutionState;
    cycles = 1;
    while (cycles--)
        cpu.clock();
    postExecutionState = captureCPUState();

    preExecutionState.stack[0]++;
    preExecutionState.ACC = 3;
    preExecutionState.CY = 0;
    compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU4004InstructionsTests, SUBTest) {
    rom[0] = 0x9C; // SUB 12 (no borrow, CY = 1)
    rom[1] = 0x9D; // SUB 13 (produces borrow, CY = 0)
    rom[2] = 0x9E; // SUB 14 (consumes and produces borrow)
    rom[3] = 0x9F; // SUB 15 (consumes borrow)
    cpu.getState().ACC = 5;
    cpu.getState().CY = 1;
    cpu.getState().regs[12] = 3;
    cpu.getState().regs[13] = 8;
    cpu.getState().regs[14] = 14;
    cpu.getState().regs[15] = 2;

    auto preExecutionState = captureCPUState();
    u8 cycles = 1;
    while (cycles--)
        cpu.clock();
    auto postExecutionState = captureCPUState();

    preExecutionState.stack[0]++;
    preExecutionState.ACC = 2;
    preExecutionState.CY = 1;
    compareCPUStates(preExecutionState, postExecutionState);

    preExecutionState = postExecutionState;
    cycles = 1;
    while (cycles--)
        cpu.clock();
    postExecutionState = captureCPUState();

    preExecutionState.stack[0]++;
    preExecutionState.ACC = 10;
    preExecutionState.CY = 0;
    compareCPUStates(preExecutionState, postExecutionState);

    preExecutionState = postExecutionState;
    cycles = 1;
    while (cycles--)
        cpu.clock();
    postExecutionState = captureCPUState();

    preExecutionState.stack[0]++;
    preExecutionState.ACC = 11;
    compareCPUStates(preExecutionState, postExecutionState);

    preExecutionState = postExecutionState;
    cycles = 1;
    while (cycles--)
        cpu.clock();
    postExecutionState = captureCPUState();

    preExecutionState.stack[0]++;
    preExecutionState.ACC = 8;
    preExecutionState.CY = 1;
    compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU4004InstructionsTests, INCTest) {
    rom[0] = 0x6C; // INC 12
    rom[1] = 0x6C; // INC 12
    cpu.getState().regs[12] = 14;

    auto preExecutionState = captureCPUState();
    u8 cycles = 1;
    while (cycles--)
        cpu.clock();
    auto postExecutionState = captureCPUState();

    preExecutionState.stack[0]++;
    preExecutionState.regs[12] = 15;
    compareCPUStates(preExecutionState, postExecutionState);

    preExecutionState = postExecutionState;
    cycles = 1;
    while (cycles--)
        cpu.clock();
    postExecutionState = captureCPUState();

    preExecutionState.stack[0]++;
    preExecutionState.regs[12] = 0;
    compareCPUStates(preExecutionState, postExecutionState);
}

TEST_F(CPU4004InstructionsTests, BBLTest) {
    rom[0] = 0xC7; // BBL 7
    cpu.getState().stack[0] = 0x123;
    cpu.getState().stack[1] = 0;
    cpu.getState().SP = 1;

    auto preExecutionState = captureCPUState();
    u8 cycles = 1;
    while (cycles--)
        cpu.clock();
    auto postExecutionState = captureCPUState();

    preExecutionState.ACC = 7;
    preExecutionState.SP = 0;
    preExecutionState.stack[1]++;
    compareCPUStates(preExecutionState, postExecutionState);
}

} // namespace
