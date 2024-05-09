#pragma once
#include "../cpu.hpp"

#include <gtest/gtest.h>

namespace PSX {

	struct CPUInstructionsTests :
		public testing::Test
	{
		CPU cpu;

		static constexpr size_t MEMORY_SIZE = 0x10;
		u32 memory[MEMORY_SIZE];

		CPU::CPUStatus cpuStatusBefore;
		CPU::COP0Status cop0StatusBefore;
		u32 memoryBefore[MEMORY_SIZE];

		CPUInstructionsTests() {
			cpu.mapRead32MemoryCallback([&](u32 address) { return memory[(address - 0xBFC00000) >> 2]; });
			cpu.mapWrite32MemoryCallback([&](u32 address, u32 data) { memory[(address - 0xBFC00000) >> 2] = data; });
		}

		void SetUp() override {
			cpu.reset();
		}

		void makeSnapshot() {
			cpuStatusBefore = cpu.getCPUStatus();
			cop0StatusBefore = cpu.getCOP0Status();
			std::memcpy(memoryBefore, memory, MEMORY_SIZE * sizeof(u32));
		}

		void checkSnapshot() const {
			auto& cpuStatus = cpu.getCPUStatus();
			EXPECT_EQ(cpuStatusBefore.PC, cpuStatus.PC);
			EXPECT_EQ(cpuStatusBefore.HI, cpuStatus.HI);
			EXPECT_EQ(cpuStatusBefore.LO, cpuStatus.LO);
			for (auto i = 0; i < 32; i++)
				EXPECT_EQ(cpuStatusBefore.regs[i], cpuStatus.regs[i]) << "i == " << i;

			auto& cop0Status = cpu.getCOP0Status();
			for (auto i = 0; i < 16; i++)
				EXPECT_EQ(cop0StatusBefore.regs[i], cop0Status.regs[i]) << "i == " << i;

			for (auto i = 0; i < MEMORY_SIZE; i++)
				EXPECT_EQ(memoryBefore[i], memory[i]) << "i == " << i;
		}
	};

} // namespace PSX
