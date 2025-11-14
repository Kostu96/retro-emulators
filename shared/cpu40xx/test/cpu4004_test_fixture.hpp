#pragma once
#include <cpu40xx/cpu40xx.hpp>

#include <gtest/gtest.h>
#include <cstring>
#include <functional>

struct CPU4004Tests :
    ::testing::Test {
    static constexpr u8 ROM_CHIPS_COUNT = 2;
    static constexpr u16 ROM_SIZE = ROM_CHIPS_COUNT * 0x100;
    static constexpr u8 RAM_BANKS_COUNT = 2; 
    static constexpr u16 RAM_DATA_SIZE = RAM_BANKS_COUNT * 0x100;
    static constexpr u16 RAM_STATUS_SIZE = RAM_BANKS_COUNT * 0x40;
    static constexpr u16 RAM_OUTPUT_SIZE = RAM_BANKS_COUNT * 4;

    struct Writables {
        u8 romSrcReg{};
        u8 romIO[ROM_CHIPS_COUNT]{};
        u8 ramSrcRegs[RAM_BANKS_COUNT]{};
        u8 ramData[RAM_DATA_SIZE]{};
        u8 ramStatus[RAM_STATUS_SIZE]{};
        u8 ramIO[RAM_OUTPUT_SIZE]{};

        bool operator==(const Writables&) const = default;
    };

    CPU4004Tests() :
        cpu(CPU40xx::Mode::Intel4004) {
        cpu.mapReadROMCallback([this](u16 address) -> u8 {
            EXPECT_LT(address, ROM_SIZE);

            if (address < ROM_SIZE) return rom[address];
            return 0;
        });

        cpu.mapWriteSRCRegisterCallback([this](u8 cmRam, u8 value) {
            EXPECT_GT(cmRam, 0);
            EXPECT_LE(cmRam, 2);

            switch (cmRam) {
            case 0b0001: writables.ramSrcRegs[0] = value; break;
            case 0b0010: writables.ramSrcRegs[1] = value; break;
            }
            writables.romSrcReg = value;
        });

        cpu.mapReadRAMDataCallback([this](u8 cmRam) -> u8 {
            EXPECT_GT(cmRam, 0);
            EXPECT_LE(cmRam, 2);

            u16 address = 0;
            switch (cmRam) {
            case 0b0001: address = writables.ramSrcRegs[0]; break;
            case 0b0010: address = writables.ramSrcRegs[1]; break;
            }
            address |= ((cmRam >> 1) << 8);
            EXPECT_LT(address, RAM_DATA_SIZE);
            return writables.ramData[address];
        });

        cpu.mapWriteRAMDataCallback([this](u8 cmRam, u8 value) {
            EXPECT_GT(cmRam, 0);
            EXPECT_LE(cmRam, 2);
            EXPECT_EQ(value & 0xF0, 0);

            u16 address = 0;
            switch (cmRam) {
            case 0b0001: address = writables.ramSrcRegs[0]; break;
            case 0b0010: address = writables.ramSrcRegs[1]; break;
            }
            address |= ((cmRam >> 1) << 8);
            EXPECT_LT(address, RAM_DATA_SIZE);
            writables.ramData[address] = value;
        });

        cpu.mapReadRAMStatusCallback([this](u8 cmRam, u8 charIdx) -> u8 {
            EXPECT_GT(cmRam, 0);
            EXPECT_LE(cmRam, 2);
            EXPECT_LE(charIdx, 4);

            u16 address = 0;
            switch (cmRam) {
            case 0b0001: address = writables.ramSrcRegs[0]; break;
            case 0b0010: address = writables.ramSrcRegs[1]; break;
            }
            address &= 0xF0;
            address >>= 2;
            address |= ((cmRam >> 1) << 6);
            address |= charIdx;
            EXPECT_LT(address, RAM_STATUS_SIZE);
            return writables.ramStatus[address];
        });

        cpu.mapWriteRAMStatusCallback([this](u8 cmRam, u8 charIdx, u8 value) {
            EXPECT_GT(cmRam, 0);
            EXPECT_LE(cmRam, 2);
            EXPECT_LE(charIdx, 4);
            EXPECT_EQ(value & 0xF0, 0);

            u16 address = 0;
            switch (cmRam) {
            case 0b0001: address = writables.ramSrcRegs[0]; break;
            case 0b0010: address = writables.ramSrcRegs[1]; break;
            }
            address &= 0xF0;
            address >>= 2;
            address |= ((cmRam >> 1) << 6);
            address |= charIdx;
            EXPECT_LT(address, RAM_STATUS_SIZE);
            writables.ramStatus[address] = value;
        });

        cpu.mapReadROMIOCallback([this]() -> u8 {
            u8 address = writables.romSrcReg;
            address >>= 4;
            return writables.romIO[address];
        });

        cpu.mapWriteROMIOCallback([this](u8 value) {
            EXPECT_EQ(value & 0xF0, 0);

            u8 address = writables.romSrcReg;
            address >>= 4;
            writables.romIO[address] = value;
        });

        cpu.mapWriteRAMIOCallback([this](u8 cmRam, u8 value) {
            EXPECT_GT(cmRam, 0);
            EXPECT_LE(cmRam, 2);
            EXPECT_EQ(value & 0xF0, 0);

            u16 address = 0;
            switch (cmRam) {
            case 0b0001: address = writables.ramSrcRegs[0]; break;
            case 0b0010: address = writables.ramSrcRegs[1]; break;
            }
            address >>= 6;
            address |= ((cmRam >> 1) << 2);
            EXPECT_LT(address, RAM_STATUS_SIZE);
            writables.ramIO[address] = value;
        });
    }

    void SetUp() override {
        memset(rom, 0, ROM_SIZE);
        writables = Writables{};
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

    void execute(u8 cycles, std::function<void(CPU40xx::State&, Writables&)> stateChanges) {
        auto preExecutionState = captureCPUState();
        Writables preWritables = writables;
        while (cycles--) cpu.clock();
        auto postExecutionState = captureCPUState();
        stateChanges(preExecutionState, preWritables);
        compareCPUStates(preExecutionState, postExecutionState);
        EXPECT_EQ(preWritables, writables);
    }

    CPU40xx cpu;
    u8 rom[ROM_SIZE]{};
    Writables writables;
};
