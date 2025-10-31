#pragma once
#include <cpu40xx/cpu40xx.hpp>

#include <gtest/gtest.h>
#include <cstring>
#include <functional>

struct CPU4004Tests :
    public ::testing::Test {

    static constexpr u8 SRC_REG_COUNT = 2; // for 2 rom chips and 2 ram banks
    static constexpr u16 ROM_SIZE = SRC_REG_COUNT * 0x100; // 2 i4001s a.k.a. 2 pages
    static constexpr u16 RAM_DATA_SIZE = SRC_REG_COUNT * 0x100; // 2 banks of 4 i4002s
    static constexpr u16 RAM_STATUS_SIZE = SRC_REG_COUNT * 0x40; // 2 banks of 4 i4002s

    struct RAM {
        u8 data[RAM_DATA_SIZE]{};
        u8 status[RAM_STATUS_SIZE]{};

        bool operator==(const RAM&) const = default;
    };

    CPU4004Tests() :
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

        cpu.mapReadRAMDataCallback([this](u8 cmRam) -> u8 {
            u16 address = 0;
            switch (cmRam) {
            case 0b0001: address = srcRegs[0]; break;
            case 0b0010: address = srcRegs[1]; break;
            }
            address |= ((cmRam >> 1) << 8);
            EXPECT_LT(address, RAM_DATA_SIZE);
            return ram.data[address];
        });

        cpu.mapWriteRAMDataCallback([this](u8 cmRam, u8 value) {
            u16 address = 0;
            switch (cmRam) {
            case 0b0001: address = srcRegs[0]; break;
            case 0b0010: address = srcRegs[1]; break;
            }
            address |= ((cmRam >> 1) << 8);
            EXPECT_LT(address, RAM_DATA_SIZE);
            EXPECT_EQ(value & 0xF0, 0);
            ram.data[address] = value;
        });

        cpu.mapReadRAMStatusCallback([this](u8 cmRam, u8 charIdx) -> u8 {
            u16 address = 0;
            switch (cmRam) {
            case 0b0001: address = srcRegs[0]; break;
            case 0b0010: address = srcRegs[1]; break;
            }
            address &= 0xF0;
            address >>= 2;
            address |= ((cmRam >> 1) << 6);
            address |= charIdx;
            EXPECT_LT(address, RAM_STATUS_SIZE);
            return ram.status[address];
        });

        cpu.mapWriteRAMStatusCallback([this](u8 cmRam, u8 charIdx, u8 value) {
            u16 address = 0;
            switch (cmRam) {
            case 0b0001: address = srcRegs[0]; break;
            case 0b0010: address = srcRegs[1]; break;
            }
            address &= 0xF0;
            address >>= 2;
            address |= ((cmRam >> 1) << 6);
            address |= charIdx;
            EXPECT_LT(address, RAM_STATUS_SIZE);
            EXPECT_EQ(value & 0xF0, 0);
            ram.status[address] = value;
        });
    }

    void SetUp() override {
        memset(rom, 0, ROM_SIZE);
        memset(ram.data, 0, RAM_DATA_SIZE);
        memset(ram.status, 0, RAM_STATUS_SIZE);
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

    void execute(u8 cycles, std::function<void(CPU40xx::State&, u8*, RAM&)> stateChanges) {
        auto preExecutionState = captureCPUState();
        u8 preSrcRegs[SRC_REG_COUNT];
        memcpy(preSrcRegs, srcRegs, SRC_REG_COUNT);
        RAM preRam = ram;

        while (cycles--) cpu.clock();

        auto postExecutionState = captureCPUState();

        stateChanges(preExecutionState, preSrcRegs, preRam);
        compareCPUStates(preExecutionState, postExecutionState);
        EXPECT_EQ(memcmp(preSrcRegs, srcRegs, SRC_REG_COUNT), 0);
        EXPECT_EQ(preRam, ram);
    }

    CPU40xx cpu;
    u8 srcRegs[SRC_REG_COUNT]{};
    u8 rom[ROM_SIZE]{};
    RAM ram;
};
