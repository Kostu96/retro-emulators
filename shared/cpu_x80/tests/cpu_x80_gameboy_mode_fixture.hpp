#pragma once
#include "cpu_x80.hpp"

#include <gtest/gtest.h>

struct CPUMockGB :
    public CPUx80<CPUx80Mode::GameBoy>
{
    using CPUx80Base::State;
    using CPUx80Base::m_state;

    CPUMockGB() : CPUx80<CPUx80Mode::GameBoy>{ CPUx80Mode::GameBoy } {}
};

struct CPUx80GameBoyModeTests :
    public testing::Test
{
    static constexpr u16 ROM_SIZE = 0x10;
    static constexpr u16 RAM_SIZE = 0x20;

    u8 rom[ROM_SIZE]{};
    u8 ram[RAM_SIZE]{};

    CPUMockGB cpu;

    CPUx80GameBoyModeTests()
    {
        cpu.mapReadMemoryCallback([&](u16 address)
            {
                if (address < ROM_SIZE) return rom[address];
                if (address >= 0xFF00) return ram[address - 0xFF00];
                if (address >= 0x8000) return ram[address - 0x8000];
                return u8{};
            });

        cpu.mapWriteMemoryCallback([&](u16 address, u8 data)
            {
                if (address >= 0xFF00) {
                    ram[address - 0xFF00] = data;
                    return;
                }

                if (address >= 0x8000) {
                    ram[address - 0x8000] = data;
                    return;
                }
            });
    }

    void SetUp() override
    {
        std::memset(rom, 0, ROM_SIZE);
        std::memset(ram, 0, RAM_SIZE);
        cpu.reset();
        cpu.m_state.F.byte = 0;
    }

    CPUMockGB::State captureCPUState()
    {
        CPUMockGB::State state;
        std::memcpy(&state, &cpu.m_state, sizeof(CPUMockGB::State));
        return state;
    }

    void compareCPUStates(const CPUMockGB::State& state1, const CPUMockGB::State& state2)
    {
        EXPECT_EQ(state1.PC, state2.PC);
        EXPECT_EQ(state1.SP, state2.SP);
        EXPECT_EQ(state1.AF, state2.AF);
        EXPECT_EQ(state1.BC, state2.BC);
        EXPECT_EQ(state1.DE, state2.DE);
        EXPECT_EQ(state1.HL, state2.HL);
    }
};
