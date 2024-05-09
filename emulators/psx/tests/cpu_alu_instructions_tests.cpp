#include "cpu_instructions_tests_fixture.hpp"

namespace PSX {

    using CPUALUInstructionsTests = CPUInstructionsTests;

	TEST_F(CPUALUInstructionsTests, ORITest)
	{
		memory[0] = 0x3508243F; // ORI $8, $8, 0x243F

		makeSnapshot();

		cpu.clock();

		cpuStatusBefore.regs[8] |= 0x243F;
		cpuStatusBefore.PC += 4;

		checkSnapshot();
	}

	TEST_F(CPUALUInstructionsTests, SSLTest)
	{
		cpu.overrideCPURegister(8, 0xDEADBEEF);
		memory[0] = 0x00084080; // SSL $8, $8, 0x2

		makeSnapshot();

		cpu.clock();

		cpuStatusBefore.regs[8] = 0x7AB6FBBC;
		cpuStatusBefore.PC += 4;

		checkSnapshot();
	}

	TEST_F(CPUALUInstructionsTests, ADDIUTest) // TODO: test overflow conditions and negative values
	{
		memory[0] = 0x24080B88; // ADDIU $8, $zero, 0xB88

		makeSnapshot();

		cpu.clock();

		cpuStatusBefore.regs[8] = 0xB88;
		cpuStatusBefore.PC += 4;

		checkSnapshot();
	}

	TEST_F(CPUALUInstructionsTests, ORTest)
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

} // namespace PSX
