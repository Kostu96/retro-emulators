#include "cpu_instructions_tests_fixture.hpp"

namespace PSX {

    using CPUALUInstructionsTests = CPUInstructionsTests;

	TEST_F(CPUALUInstructionsTests, SSLTest)
	{
		cpu.overrideCPURegister(8, 0xDEADBEEF);
		memory[0] = 0x00084100; // SSL $8, $8, 4

		makeSnapshot();

		cpu.clock();

		cpuStatusBefore.regs[8] = 0xEADBEEF0;
		cpuStatusBefore.PC += 4;

		checkSnapshot();
	}

	TEST_F(CPUALUInstructionsTests, SRLTestWithSignBitOn)
	{
		cpu.overrideCPURegister(8, 0xDEADBEEF);
		memory[0] = 0x00084102; // SRL $8, $8, 4

		makeSnapshot();

		cpu.clock();

		cpuStatusBefore.regs[8] = 0x0DEADBEE;
		cpuStatusBefore.PC += 4;

		checkSnapshot();
	}

	TEST_F(CPUALUInstructionsTests, SRLTestWithSignBitOff)
	{
		cpu.overrideCPURegister(8, 0x5EADBEEF);
		memory[0] = 0x00084102; // SRL $8, $8, 4

		makeSnapshot();

		cpu.clock();

		cpuStatusBefore.regs[8] = 0x05EADBEE;
		cpuStatusBefore.PC += 4;

		checkSnapshot();
	}

	TEST_F(CPUALUInstructionsTests, SRATestWithSignBitOn)
	{
		cpu.overrideCPURegister(8, 0xDEADBEEF);
		memory[0] = 0x00084103; // SRA $8, $8, 4

		makeSnapshot();

		cpu.clock();

		cpuStatusBefore.regs[8] = 0xFDEADBEE;
		cpuStatusBefore.PC += 4;

		checkSnapshot();
	}

	TEST_F(CPUALUInstructionsTests, SRATestWithSignBitOff)
	{
		cpu.overrideCPURegister(8, 0x5EADBEEF);
		memory[0] = 0x00084103; // SRA $8, $8, 4

		makeSnapshot();

		cpu.clock();

		cpuStatusBefore.regs[8] = 0x05EADBEE;
		cpuStatusBefore.PC += 4;

		checkSnapshot();
	}

	TEST_F(CPUALUInstructionsTests, SSLVTest)
	{
		cpu.overrideCPURegister(8, 0xDEADBEEF);
		cpu.overrideCPURegister(9, 4);
		memory[0] = 0x01284004; // SSLV $8, $8, $9

		makeSnapshot();

		cpu.clock();

		cpuStatusBefore.regs[8] = 0xEADBEEF0;
		cpuStatusBefore.PC += 4;

		checkSnapshot();
	}

	TEST_F(CPUALUInstructionsTests, ORITest)
	{
		memory[0] = 0x3508243F; // ORI $8, $8, 0x243F

		makeSnapshot();

		cpu.clock();

		cpuStatusBefore.regs[8] |= 0x243F;
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
