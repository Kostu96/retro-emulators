#include <cpu40xx/cpu40xx.hpp>

#include <gtest/gtest.h>
#include <cstring>
#include <functional>

namespace {

struct CPU4004InstructionsTests :
    public ::testing::Test {
        
    CPU4004InstructionsTests() :
        cpu(CPU40xx::Mode::Intel4004) {
        cpu.mapReadROMCallback([this](u16 address) -> u8 {
            if (address < ROM_SIZE) return rom[address];
            return 0;
        });
        cpu.mapWriteSRCRegisterCallback([this](u8 cmRam, u8 value) {
            switch (cmRam) {
            case 0b0001: srcRegs[0] = value; break;
            case 0b0010: srcRegs[1] = value; break;
            }
        });
    }
    
    void SetUp() override {
        memset(rom, 0, ROM_SIZE);
        memset(ram, 0, RAM_SIZE);
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
        cpu.getState().ACC = 5;
    }
    
    void TearDown() override {}
    
    CPU40xx::State captureCPUState() const {
        return cpu.getState();
    }
    
    void compareCPUStates(const CPU40xx::State& state1, const CPU40xx::State& state2) const {
        EXPECT_EQ(state1.regs, state2.regs);
        EXPECT_EQ(state1.stack, state2.stack);
        EXPECT_EQ(state1.ACC, state2.ACC);
        EXPECT_EQ(state1.SP, state2.SP);
        EXPECT_EQ(state1.CY, state2.CY);
        EXPECT_EQ(state1.test, state2.test);
        EXPECT_EQ(state1.CMRAM, state2.CMRAM);
    }
    
    void execute(u8 cycles, std::function<void(CPU40xx::State&, u8*, u8*)> stateChanges) {
        auto preExecutionState = captureCPUState();
        u8 preSrcRegs[SRC_REG_COUNT];
        memcpy(preSrcRegs, srcRegs, SRC_REG_COUNT);
        u8 preRam[RAM_SIZE];
        memcpy(preRam, ram, RAM_SIZE);
        
        while (cycles--) cpu.clock();
        
        auto postExecutionState = captureCPUState();
        u8 postSrcRegs[SRC_REG_COUNT];
        memcpy(postSrcRegs, srcRegs, SRC_REG_COUNT);
        u8 postRam[RAM_SIZE];
        memcpy(postRam, ram, RAM_SIZE);
        
        stateChanges(preExecutionState, preSrcRegs, preRam);
        compareCPUStates(preExecutionState, postExecutionState);
        EXPECT_EQ(memcmp(preSrcRegs, postSrcRegs, SRC_REG_COUNT), 0);
        EXPECT_EQ(memcmp(preRam, postRam, RAM_SIZE), 0);
    }

    static constexpr u8 SRC_REG_COUNT = 2; // for 2 rom chips and 2 ram banks
    static constexpr u16 ROM_SIZE = SRC_REG_COUNT * 0x100; // 2 i4001s a.k.a. 2 pages
    static constexpr u16 RAM_SIZE = SRC_REG_COUNT * 0x100; // 2 banks of 4 i4002s

    CPU40xx cpu;
    u8 srcRegs[SRC_REG_COUNT]{};
    u8 rom[ROM_SIZE]{};
    u8 ram[RAM_SIZE]{};
};

TEST_F(CPU4004InstructionsTests, NOPTest) {
    rom[0] = 0x0; // NOP

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
    });
}

TEST_F(CPU4004InstructionsTests, LDMTest) {
    rom[0] = 0xDA; // LDM 10

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 10;
    });
}

TEST_F(CPU4004InstructionsTests, LDTest) {
    rom[0] = 0xAC; // LD 12

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 12;
    });
}

TEST_F(CPU4004InstructionsTests, XCHTest) {
    rom[0] = 0xBC; // XCH 12
    cpu.getState().ACC = 4;

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 12;
        state.regs[12] = 4;
    });
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

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 9;
    });

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 1;
        state.CY = 1;
    });

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 0;
    });

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 3;
        state.CY = 0;
    });
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

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 2;
        state.CY = 1;
    });

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 10;
        state.CY = 0;
    });

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 11;
    });

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 8;
        state.CY = 1;
    });
}

TEST_F(CPU4004InstructionsTests, INCTest) {
    rom[0] = 0x6C; // INC 12
    rom[1] = 0x6C; // INC 12
    cpu.getState().regs[12] = 14;

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.regs[12] = 15;
    });

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.regs[12] = 0;
    });
}

TEST_F(CPU4004InstructionsTests, BBLTest) {
    rom[0] = 0xC7; // BBL 7
    rom[5] = 0xC0; // BBL 0
    cpu.getState().stack[0] = 5;
    cpu.getState().stack[1] = 0;
    cpu.getState().SP = 1;

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[1]++;
        state.ACC = 7;
        state.SP = 0;
    });

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 0;
        state.SP = 3;
    });
}

TEST_F(CPU4004InstructionsTests, JINTest) {
    rom[0] = 0x31; // JIN 0P
    rom[0xFF] = 0x33; // JIN 1P
    cpu.getState().regs[0] = 0xF;
    cpu.getState().regs[1] = 0xF;
    cpu.getState().regs[2] = 0x1;
    cpu.getState().regs[3] = 0x2;

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0] = 0xFF;
    });

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0] = 0x121;
    });
}

TEST_F(CPU4004InstructionsTests, SRCTest) {
    rom[0] = 0x21; // SRC 0P
    rom[1] = 0x21; // SRC 0P
    cpu.getState().regs[0] = 0x2;
    cpu.getState().regs[1] = 0x4;

    cpu.getState().CMRAM = 0b0001;

    execute(1, [](CPU40xx::State& state, u8* srcRegs_, u8* /*ram_*/) {
        state.stack[0]++;
        srcRegs_[0] = 0x42;
    });

    cpu.getState().CMRAM = 0b0010;

    execute(1, [](CPU40xx::State& state, u8* srcRegs_, u8* /*ram_*/) {
        state.stack[0]++;
        srcRegs_[1] = 0x42;
    });
}

TEST_F(CPU4004InstructionsTests, FINTest) {
    rom[0xFE] = 0x30; // FIN 0P
    rom[0xFF] = 0x32; // FIN 1P
    rom[0x42] = 0x12;
    rom[0x112] = 0x21;
    cpu.getState().regs[0] = 0x2;
    cpu.getState().regs[1] = 0x4;
    cpu.getState().stack[0] = 0xFE;

    // TODO(Kostu): this should take 2 cycles
    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.regs[0] = 0x2;
        state.regs[1] = 0x1;
    });

    // TODO(Kostu): this should take 2 cycles
    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.regs[2] = 0x1;
        state.regs[3] = 0x2;
    });
}

TEST_F(CPU4004InstructionsTests, JUNTest) {
    rom[0] = 0x41;
    rom[1] = 0x23; // JUN 0x123

    // TODO(Kostu): this should take 2 cycles
    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0] = 0x123;
    });
}

TEST_F(CPU4004InstructionsTests, JMSTest) {
    rom[0] = 0x51;
    rom[1] = 0x23;     // JMS 0x123
    rom[0x123] = 0x54;
    rom[0x124] = 0x56; // JMS 0
    cpu.getState().stack[2] = 0;
    cpu.getState().SP = 2;

    // TODO(Kostu): this should take 2 cycles
    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[2] += 2;
        state.stack[3] = 0x123;
        state.SP = 3;
    });

    // TODO(Kostu): this should take 2 cycles
    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[3] += 2;
        state.stack[0] = 0x456;
        state.SP = 0;
    });
}

struct JCNParametrizedTests :
    public CPU4004InstructionsTests,
    public ::testing::WithParamInterface<std::tuple<u8, u8>>
{};

TEST_P(JCNParametrizedTests, JCNConditionsTest) {
    u8 condition = std::get<0>(GetParam());

    rom[0] = 0x10 | condition;
    rom[1] = 0x42;

    bool invert = (condition >> 3) & 1;
    condition &= 0x07;
    u8 flags = std::get<1>(GetParam());
    bool clearACC = (flags >> 2) & 1;
    bool setCarry = (flags >> 1) & 1;
    bool clearTest = flags & 1;

    cpu.getState().ACC = clearACC ? 0 : 5;
    cpu.getState().CY = setCarry ? 1 : 0;
    cpu.getState().test = clearTest ? 0 : 1;

    u8 test = condition & flags;
    bool shouldJump = invert ^ static_cast<bool>(test);

    // TODO(Kostu): this should take 2 cycles
    execute(1, [shouldJump](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        if (shouldJump)
            state.stack[0] = 0x042;
        else
            state.stack[0] += 2;
    });
}

INSTANTIATE_TEST_SUITE_P(, JCNParametrizedTests,
    ::testing::Combine(
        ::testing::Range<u8>(0, 16),
        ::testing::Range<u8>(0, 8)),
    [](const ::testing::TestParamInfo<JCNParametrizedTests::ParamType>& paramInfo) {
        u8 condition = std::get<0>(paramInfo.param);
        u8 flags = std::get<1>(paramInfo.param);
        bool invert = (condition >> 3) & 1;
        condition &= 0x07;
        std::stringstream ss;
        if (condition == 0) ss << (invert ? "IvertedNo" : "No");
        ss << "Condition";
        if ((condition >> 2) & 1) ss << (invert ? "ACCIsNotZero" : "ACCIsZero");
        if ((condition >> 1) & 1) ss << (invert ? "CarryIsNotSet" : "CarryIsSet");
        if ((condition >> 0) & 1) ss << (invert ? "TestIsNotZero" : "TestIsZero");
        ss << "_State";
        ss << ((flags >> 2) & 1 ? "ACCIsZero" : "ACCIsNotZero");
        ss << ((flags >> 1) & 1 ? "CarryIsSet" : "CarryIsNotSet");
        ss << ((flags >> 0) & 1 ? "TestIsZero" : "TestIsNotZero");
        return ss.str();
    }
);

TEST_F(CPU4004InstructionsTests, JCNEndOfPageTest) {
    rom[0xFE] = 0x18;
    rom[0xFF] = 0x42; // JCN 8, 0x42 - will always jump

    cpu.getState().stack[0] = 0xFE;

    // TODO(Kostu): this should take 2 cycles
    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0] = 0x142;
    });
}

TEST_F(CPU4004InstructionsTests, ISZTest) {
    rom[0x0] = 0x78;
    rom[0x1] = 0x0;   // ISZ 8, 0x0
    rom[0x2] = 0x79;
    rom[0x3] = 0xFE;  // ISZ 9, 0xFE
    rom[0xFE] = 0x79;
    rom[0xFF] = 0x42; // ISZ 9, 0x42

    cpu.getState().regs[8] = 10;
    cpu.getState().regs[9] = 13;

    // TODO(Kostu): this should take 2 cycles
    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.regs[8]++;
    });

    // TODO(Kostu): this should take 10 cycles
    execute(5, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.regs[8] = 0;
        state.stack[0] += 2;
    });

    // TODO(Kostu): this should take 2 cycles
    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.regs[9]++;
        state.stack[0] = 0xFE;
    });

    // TODO(Kostu): this should take 2 cycles
    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.regs[9]++;
        state.stack[0] = 0x142;
    });
}

TEST_F(CPU4004InstructionsTests, FIMTest) {
    rom[0] = 0x22;
    rom[1] = 0x42; // FIM 1P, 0x42

    // TODO(Kostu): this should take 2 cycles
    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0] += 2;
        state.regs[2] = 0x2;
        state.regs[3] = 0x4;
    });
}

TEST_F(CPU4004InstructionsTests, RDMTest) {
    GTEST_SKIP() << "Not implemented!";
    // TODO(Kostu): implement after DCL SRC and stuff
}

TEST_F(CPU4004InstructionsTests, RDxTest) {
    GTEST_SKIP() << "Not implemented!";
    // TODO(Kostu): implement after DCL SRC and stuff
}

TEST_F(CPU4004InstructionsTests, RDRTest) {
    GTEST_SKIP() << "Not implemented!";
    // TODO(Kostu): implement after DCL SRC and stuff
}

TEST_F(CPU4004InstructionsTests, WRMTest) {
    GTEST_SKIP() << "Not implemented!";
    // TODO(Kostu): implement after DCL SRC and stuff
}

TEST_F(CPU4004InstructionsTests, WRxTest) {
    GTEST_SKIP() << "Not implemented!";
    // TODO(Kostu): implement after DCL SRC and stuff
}

TEST_F(CPU4004InstructionsTests, WRRTest) {
    GTEST_SKIP() << "Not implemented!";
    // TODO(Kostu): implement after DCL SRC and stuff
}

TEST_F(CPU4004InstructionsTests, WMPTest) {
    GTEST_SKIP() << "Not implemented!";
    // TODO(Kostu): implement after DCL SRC and stuff
}

TEST_F(CPU4004InstructionsTests, ADMTest) {
    GTEST_SKIP() << "Not implemented!";
    // TODO(Kostu): implement after DCL SRC and stuff
}

TEST_F(CPU4004InstructionsTests, SBMTest) {
    GTEST_SKIP() << "Not implemented!";
    // TODO(Kostu): implement after DCL SRC and stuff
}

TEST_F(CPU4004InstructionsTests, CLBTest) {
    rom[0] = 0xF0; // CLB

    cpu.getState().ACC = 6;
    cpu.getState().CY = 1;

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 0;
        state.CY = 0;
    });
}

TEST_F(CPU4004InstructionsTests, CLCTest) {
    rom[0] = 0xF1; // CLC

    cpu.getState().CY = 1;

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.CY = 0;
    });
}

TEST_F(CPU4004InstructionsTests, CMCTest) {
    rom[0] = 0xF3; // CMC
    rom[1] = 0xF3; // CMC

    cpu.getState().CY = 1;

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.CY = 0;
    });

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.CY = 1;
    });
}

TEST_F(CPU4004InstructionsTests, STCTest) {
    rom[0] = 0xFA; // STC

    cpu.getState().CY = 0;

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.CY = 1;
    });
}

TEST_F(CPU4004InstructionsTests, CMATest) {
    rom[0] = 0xF4; // CMA

    cpu.getState().ACC = 10;

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 5;
    });
}

TEST_F(CPU4004InstructionsTests, IACTest) {
    rom[0] = 0xF2; // IAC
    rom[1] = 0xF2; // IAC

    cpu.getState().ACC = 14;
    cpu.getState().CY = 0;

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 15;
    });

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 0;
        state.CY = 1;
    });
}

TEST_F(CPU4004InstructionsTests, DACTest) {
    rom[0] = 0xF8; // DAC
    rom[1] = 0xF8; // DAC

    cpu.getState().ACC = 1;
    cpu.getState().CY = 0;

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 0;
        state.CY = 1;
    });

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 15;
        state.CY = 0;
    });
}

TEST_F(CPU4004InstructionsTests, RALTest) {
    rom[0] = 0xF5; // RAL
    rom[1] = 0xF5; // RAL

    cpu.getState().ACC = 10;
    cpu.getState().CY = 0;

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 4;
        state.CY = 1;
    });

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 9;
        state.CY = 0;
    });
}

TEST_F(CPU4004InstructionsTests, RARTest) {
    rom[0] = 0xF6; // RAR
    rom[1] = 0xF6; // RAR

    cpu.getState().ACC = 5;
    cpu.getState().CY = 0;

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 2;
        state.CY = 1;
    });

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 9;
        state.CY = 0;
    });
}

TEST_F(CPU4004InstructionsTests, TCCTest) {
    rom[0] = 0xF7; // TCC
    rom[1] = 0xF7; // TCC

    cpu.getState().ACC = 6;
    cpu.getState().CY = 1;

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 1;
        state.CY = 0;
    });

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 0;
    });
}

TEST_F(CPU4004InstructionsTests, DAATest) {
    rom[0] = 0xFB; // DAA
    rom[1] = 0xFB; // DAA
    rom[2] = 0xFB; // DAA
    rom[3] = 0xFB; // DAA
    rom[4] = 0xFB; // DAA

    cpu.getState().ACC = 5;
    cpu.getState().CY = 0;

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
    });

    cpu.getState().ACC = 10;

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 0;
        state.CY = 1;
    });

    cpu.getState().ACC = 3;

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 9;
        state.CY = 0;
    });

    cpu.getState().ACC = 12;
    cpu.getState().CY = 1;

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 2;
        state.CY = 1;
    });

    cpu.getState().ACC = 15;
    cpu.getState().CY = 0;

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 5;
        state.CY = 1;
    });
}

TEST_F(CPU4004InstructionsTests, TCSTest) {
    rom[0] = 0xF9; // TCS
    rom[1] = 0xF9; // TCS

    cpu.getState().CY = 1;

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 10;
        state.CY = 0;
    });

    execute(1, [](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = 9;
    });
}

struct KBPAndDCLParametrizedTests :
    public CPU4004InstructionsTests,
    public ::testing::WithParamInterface<u8>
{};

TEST_P(KBPAndDCLParametrizedTests, KBPTest) {
    u8 value = GetParam();

    rom[0] = 0xFC; // KBP

    cpu.getState().ACC = value;

    u8 result = [](u8 val) -> u8 {
        switch (val) {
        case 0b0000: return 0;
        case 0b0001: return 1;
        case 0b0010: return 2;
        case 0b0100: return 3;
        case 0b1000: return 4;
        default: return 0b1111;
        }
    }(value);

    execute(1, [result](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.ACC = result;
    });
}

TEST_P(KBPAndDCLParametrizedTests, DCLTest) {
    u8 value = GetParam();

    rom[0] = 0xFD; // DCL

    cpu.getState().ACC = value;

    u8 result = [](u8 val) -> u8 {
        switch (val & 0b111) {
        case 0b000: return 0b0001;
        case 0b001: return 0b0010;
        case 0b010: return 0b0100;
        case 0b100: return 0b1000;
        case 0b011: return 0b0110;
        case 0b101: return 0b1010;
        case 0b110: return 0b1100;
        case 0b111: return 0b1110;
        }
        return 0b1111;
    }(value);
    ASSERT_NE(result, 0b1111);

    execute(1, [result](CPU40xx::State& state, u8* /*srcRegs_*/, u8* /*ram_*/) {
        state.stack[0]++;
        state.CMRAM = result;
    });
}

INSTANTIATE_TEST_SUITE_P(, KBPAndDCLParametrizedTests, ::testing::Range<u8>(0, 16));

// TODO(Kostu): assembler test programs? separate cpp file?
TEST_F(CPU4004InstructionsTests, SubroutineTest) {
    // TODO(Kostu):
}

TEST_F(CPU4004InstructionsTests, CounterLoopTest) {
    // TODO(Kostu): 8 bit ISZ loop
}

TEST_F(CPU4004InstructionsTests, BCDMathTest) {
    // TODO(Kostu):
}

} // namespace
