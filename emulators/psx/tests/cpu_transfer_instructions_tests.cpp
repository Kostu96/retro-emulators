#include "cpu_instructions_tests_fixture.hpp"

namespace PSX {

	using CPUTransferInstructionsTests = CPUInstructionsTests;

	TEST_F(CPUTransferInstructionsTests, LUITest)
	{
		memory[0] = 0x3C080013; // LUI $8, 0x13

		makeSnapshot();

		cpu.clock();

		cpuStatusBefore.regs[8] = 0x13 << 16;
		cpuStatusBefore.PC += 4;

		checkSnapshot();
	}

	TEST_F(CPUTransferInstructionsTests, SWTestWithPositiveOffset)
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

	TEST_F(CPUTransferInstructionsTests, SWTestWithNegativeOffset)
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

	TEST_F(CPUTransferInstructionsTests, LWTestWithPositiveOffset)
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

	TEST_F(CPUTransferInstructionsTests, LWTestWithNegativeOffset)
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

	TEST_F(CPUTransferInstructionsTests, MTC0Test)
	{
		cpu.overrideCPURegister(12, 0xDEADBEEF);
		memory[0] = 0x408C6000; // MTC0 $12, $cop0_sr

		makeSnapshot();

		cpu.clock();

		cop0StatusBefore.SR.value = 0xDEADBEEF;
		cpuStatusBefore.PC += 4;

		checkSnapshot();
	}

} // namespace PSX
