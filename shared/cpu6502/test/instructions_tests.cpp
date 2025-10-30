#include "cpu6502/cpu6502.hpp"
#include "utils/file_io.hpp"

#include <gtest/gtest.h>

struct CPU6502Tests :
    public testing::Test {
    static constexpr size_t RAM_SIZE = 0x10000;

    u8 ram[RAM_SIZE]{};

    CPU6502 cpu;

    CPU6502Tests() {
        cpu.mapReadMemoryCallback([&](u16 address) { return ram[address]; });
        cpu.mapWriteMemoryCallback([&](u16 address, u8 data) { ram[address] = data; });
    }

    void SetUp() override {
        cpu.reset();
    }

    void TearDown() override {}
};

TEST_F(CPU6502Tests, GivenROMFileWith6502FunctionalTestWhenExecutingUntilEndlessLoopThenIsSuccess) {
    size_t size = RAM_SIZE;
    ASSERT_TRUE(readFile("test_files/6502/6502_functional_test.bin", reinterpret_cast<char*>(ram), size, true));

    cpu.setPC(0x0400);

    cpu.runUntilEndlessLoop();

    EXPECT_EQ(cpu.getPC(), 0x3469);
}
