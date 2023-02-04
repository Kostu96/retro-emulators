#pragma once
#include "cpu8080.hpp"

#include <gtest/gtest.h>

struct CPU8080GameBoyModeTests :
	public testing::Test
{
	static constexpr u16 ROM_SIZE = 0x10;
	static constexpr u16 RAM_SIZE = 0x20;

	u8 rom[ROM_SIZE]{};
	u8 ram[RAM_SIZE]{};

	CPU8080 CPU;

	CPU8080GameBoyModeTests() :
		CPU{ CPU8080::Mode::GameBoy }
	{
		CPU.mapReadMemoryCallback([&](u16 address)
			{
				if (address < ROM_SIZE) return rom[address];
		if (address >= 0xFF00) return ram[address - 0xFF00];
		if (address >= 0x8000) return ram[address - 0x8000];
		return u8{};
			});

		CPU.mapWriteMemoryCallback([&](u16 address, u8 data)
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
		CPU.reset();
		CPU.m_state.F.byte = 0;
	}

	CPU8080::State captureCPUState()
	{
		CPU8080::State state;
		std::memcpy(&state, &CPU.m_state, sizeof(CPU8080::State));
		return state;
	}

	void compareCPUStates(const CPU8080::State& state1, const CPU8080::State& state2)
	{
		EXPECT_EQ(state1.PC, state2.PC);
		EXPECT_EQ(state1.SP, state2.SP);
		EXPECT_EQ(state1.AF, state2.AF);
		EXPECT_EQ(state1.BC, state2.BC);
		EXPECT_EQ(state1.DE, state2.DE);
		EXPECT_EQ(state1.HL, state2.HL);
	}
};
