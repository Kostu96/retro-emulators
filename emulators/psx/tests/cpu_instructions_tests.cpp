#include "../cpu.hpp"

#include <gtest/gtest.h>

namespace PSX {

	struct CPUTests :
		public testing::Test
	{
		CPU cpu;

		static constexpr size_t MEMORY_SIZE = 0x10;
		u32 memory[MEMORY_SIZE];

		CPU::CPUStatus cpuStatusBefore;
		CPU::COP0Status cop0StatusBefore;
		u32 memoryBefore[MEMORY_SIZE];

		CPUTests() {
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

		void checkSnapshot() {
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

	TEST_F(CPUTests, LUITest)
	{
		memory[0] = 0x3C080013; // LUI $8, 0x13

		makeSnapshot();

		cpu.clock();

		cpuStatusBefore.regs[8] = 0x13 << 16;
		cpuStatusBefore.PC += 4;

		checkSnapshot();
	}

	TEST_F(CPUTests, ORITest)
	{
		memory[0] = 0x3508243F; // ORI $8, $8, 0x243F

		makeSnapshot();

		cpu.clock();

		cpuStatusBefore.regs[8] |= 0x243F;
		cpuStatusBefore.PC += 4;

		checkSnapshot();
	}

	TEST_F(CPUTests, SWTestWithPositiveOffset)
	{
		cpu.overrideCPURegister(1, 0xBFC00002);
		cpu.overrideCPURegister(8, 0xDEADBEEF);
		memory[0] = 0xAC280006; // SW $8, 0x6($1)

		makeSnapshot();

		cpu.clock();

		cpuStatusBefore.PC += 4;
		memoryBefore[0x2] = 0xDEADBEEF;

		checkSnapshot();
	}

	TEST_F(CPUTests, SWTestWithNegativeOffset)
	{
		cpu.overrideCPURegister(1, 0xBFC00009);
		cpu.overrideCPURegister(8, 0xDEADBEEF);
		memory[0] = 0xAC28FFFF; // SW $8, 0xFFFF($1)

		makeSnapshot();

		cpu.clock();

		cpuStatusBefore.PC += 4;
		memoryBefore[0x2] = 0xDEADBEEF;

		checkSnapshot();
	}

	TEST_F(CPUTests, LWTestWithPositiveOffset)
	{
		cpu.overrideCPURegister(1, 0xBFC00002);
		memory[0] = 0x8C280006; // LW $8, 0x6($1)
		memory[1] = 0x0;        // NOP
		memory[2] = 0xDEADBEEF;
		makeSnapshot();

		cpu.clock(); // execute load
		cpu.clock(); // wait for load delay slot

		cpuStatusBefore.regs[8] = 0xDEADBEEF;
		cpuStatusBefore.PC += 8;
		
		checkSnapshot();
	}

	TEST_F(CPUTests, LWTestWithNegativeOffset)
	{
		cpu.overrideCPURegister(1, 0xBFC00009);
		memory[0] = 0x8C28FFFF; // LW $8, 0xFFFF($1)
		memory[1] = 0x0;        // NOP
		memory[2] = 0xDEADBEEF;

		makeSnapshot();

		cpu.clock(); // execute load
		cpu.clock(); // wait for load delay slot

		cpuStatusBefore.regs[8] = 0xDEADBEEF;
		cpuStatusBefore.PC += 8;

		checkSnapshot();
	}

	// TODO: test LW load delay slots behavior

	TEST_F(CPUTests, SSLTest)
	{
		cpu.overrideCPURegister(8, 0xDEADBEEF);
		memory[0] = 0x00084080; // SSL $8, $8, 0x2
		
		makeSnapshot();

		cpu.clock();

		cpuStatusBefore.regs[8] = 0x7AB6FBBC;
		cpuStatusBefore.PC += 4;

		checkSnapshot();
	}

	TEST_F(CPUTests, ADDIUTest) // TODO: test overflow conditions and negative values
	{
		memory[0] = 0x24080B88; // ADDIU $8, $zero, 0xB88

		makeSnapshot();

		cpu.clock();

		cpuStatusBefore.regs[8] = 0xB88;
		cpuStatusBefore.PC += 4;

		checkSnapshot();
	}
	
	TEST_F(CPUTests, JTest)
	{
		memory[0] = 0x0BF00004; // J 0xFC00010
		memory[1] = 0x24080123; // ADDIU $8, $zero, 0x123
		memory[4] = 0x24090456; // ADDIU $9, $zero, 0x456

		makeSnapshot();

		cpu.clock(); // execute jump
		cpu.clock(); // execute branch delay slot
		cpu.clock(); // execute instruction after jump

		cpuStatusBefore.regs[8] = 0x123;
		cpuStatusBefore.regs[9] = 0x456;
		cpuStatusBefore.PC = 0xBFC00014;

		checkSnapshot();
	}

	TEST_F(CPUTests, ORTest)
	{
		cpu.overrideCPURegister(1, 0x12345678);
		cpu.overrideCPURegister(2, 0x87654321);
		memory[0] = 0x00221825; // OR $3, $1, $2

		makeSnapshot();

		cpu.clock();

		cpuStatusBefore.regs[3] = 0x97755779;
		cpuStatusBefore.PC += 4;

		checkSnapshot();
	}

	TEST_F(CPUTests, MTC0Test)
	{
		cpu.overrideCPURegister(12, 0xDEADBEEF);
		memory[0] = 0x408C6000; // MTC0 $12, $cop0_sr

		makeSnapshot();

		cpu.clock();

		cop0StatusBefore.SR.value = 0xDEADBEEF;
		cpuStatusBefore.PC += 4;

		checkSnapshot();
	}

	TEST_F(CPUTests, BNETestWithRegistersEqual)
	{
		cpu.overrideCPURegister(1, 0xDEADBEEF);
		cpu.overrideCPURegister(2, 0xDEADBEEF);
		memory[0] = 0x14220003; // 0x00: BNE $1, $2, 12
		memory[1] = 0x24080123; // 0x04: ADDIU $8, $zero, 0x123
		memory[2] = 0x24090456; // 0x08: ADDIU $9, $zero, 0x456
		memory[4] = 0x24090456; // 0x10: ADDIU $7, $zero, 0x789

		makeSnapshot();

		cpu.clock(); // execute branch not taken
		cpu.clock(); // execute branch delay slot
		cpu.clock(); // execute instruction after

		cpuStatusBefore.regs[8] = 0x123;
		cpuStatusBefore.regs[9] = 0x456;
		cpuStatusBefore.PC += 12;

		checkSnapshot();
	}

	TEST_F(CPUTests, BNETestWithRegistersNotEqualAndPositiveOffset)
	{
		cpu.overrideCPURegister(1, 0xDEADBEEF);
		cpu.overrideCPURegister(2, 0x42);
		memory[0] = 0x14220003; // 0x00: BNE $1, $2, 12
		memory[1] = 0x24080123; // 0x04: ADDIU $8, $zero, 0x123
		memory[4] = 0x24090456; // 0x10: ADDIU $9, $zero, 0x456

		makeSnapshot();

		cpu.clock(); // execute branch
		cpu.clock(); // execute branch delay slot
		cpu.clock(); // execute instruction after jump

		cpuStatusBefore.regs[8] = 0x123;
		cpuStatusBefore.regs[9] = 0x456;
		cpuStatusBefore.PC = 0xBFC00014;

		checkSnapshot();
	}

	TEST_F(CPUTests, BNETestWithRegistersNotEqualAndNegativeOffset)
	{
		cpu.overrideCPURegister(1, 0xDEADBEEF);
		cpu.overrideCPURegister(2, 0x42);
		cpu.overrideCPURegister(7, 0);
		memory[0] = 0x24E70111; // 0x00: ADDIU $7, $7, 0x111
		memory[1] = 0x1422FFFE; // 0x04: BNE $1, $2, -8
		memory[2] = 0x24080123; // 0x08: ADDIU $8, $zero, 0x123
		memory[3] = 0x24090456; // 0x0C: ADDIU $9, $zero, 0x456

		makeSnapshot();

		cpu.clock(); // execute first ADDIU
		cpu.clock(); // execute branch
		cpu.clock(); // execute branch delay slot
		cpu.clock(); // execute second ADDIU

		cpuStatusBefore.regs[7] = 0x222;
		cpuStatusBefore.regs[8] = 0x123;
		cpuStatusBefore.PC = 0xBFC00004;

		checkSnapshot();
	}

} // namespace PSX
