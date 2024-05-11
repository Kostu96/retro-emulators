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

	TEST_F(CPUALUInstructionsTests, DIVTestWithValidPositiveArguments)
	{
		cpu.overrideCPURegister(8, 25);
		cpu.overrideCPURegister(9, 4);
		memory[0] = 0x0109001A; // DIV $8, $9

		makeSnapshot();

		cpu.clock();

		cpuStatusBefore.HI = 1;
		cpuStatusBefore.LO = 6;
		cpuStatusBefore.PC += 4;

		checkSnapshot();
	}

	TEST_F(CPUALUInstructionsTests, DIVTestWithValidNegativeArguments)
	{
		cpu.overrideCPURegister(8, -25);
		cpu.overrideCPURegister(9, 4);
		memory[0] = 0x0109001A; // DIV $8, $9

		makeSnapshot();

		cpu.clock();

		cpuStatusBefore.HI = -1;
		cpuStatusBefore.LO = -6;
		cpuStatusBefore.PC += 4;

		checkSnapshot();
	}

	TEST_F(CPUALUInstructionsTests, DIVTestWithPositiveNumeratorAnd0Denominator)
	{
		cpu.overrideCPURegister(8, 25);
		cpu.overrideCPURegister(9, 0);
		memory[0] = 0x0109001A; // DIV $8, $9

		makeSnapshot();

		cpu.clock();

		cpuStatusBefore.HI = 25;
		cpuStatusBefore.LO = -1;
		cpuStatusBefore.PC += 4;

		checkSnapshot();
	}

	TEST_F(CPUALUInstructionsTests, DIVTestWithValidNegativeNumeratorAnd0Denominator)
	{
		cpu.overrideCPURegister(8, -25);
		cpu.overrideCPURegister(9, 0);
		memory[0] = 0x0109001A; // DIV $8, $9

		makeSnapshot();

		cpu.clock();

		cpuStatusBefore.HI = -25;
		cpuStatusBefore.LO = 1;
		cpuStatusBefore.PC += 4;

		checkSnapshot();
	}

	TEST_F(CPUALUInstructionsTests, DIVTestWithValidLowestNegativeNumeratorAndHighestNegativeDenominator)
	{
		cpu.overrideCPURegister(8, 0x80000000);
		cpu.overrideCPURegister(9, -1);
		memory[0] = 0x0109001A; // DIV $8, $9

		makeSnapshot();

		cpu.clock();

		cpuStatusBefore.HI = 0;
		cpuStatusBefore.LO = 0x80000000;
		cpuStatusBefore.PC += 4;

		checkSnapshot();
	}

	TEST_F(CPUALUInstructionsTests, DIVUTestWithValidArguments)
	{
		cpu.overrideCPURegister(8, 25);
		cpu.overrideCPURegister(9, 4);
		memory[0] = 0x0109001B; // DIVU $8, $9

		makeSnapshot();

		cpu.clock();

		cpuStatusBefore.HI = 1;
		cpuStatusBefore.LO = 6;
		cpuStatusBefore.PC += 4;

		checkSnapshot();
	}

	TEST_F(CPUALUInstructionsTests, DIVUTestWith0Denominator)
	{
		cpu.overrideCPURegister(8, 25);
		cpu.overrideCPURegister(9, 0);
		memory[0] = 0x0109001B; // DIVU $8, $9

		makeSnapshot();

		cpu.clock();

		cpuStatusBefore.HI = 25;
		cpuStatusBefore.LO = -1;
		cpuStatusBefore.PC += 4;

		checkSnapshot();
	}

	TEST_F(CPUALUInstructionsTests, ADDTestWithValidPositiveArguments)
	{
		cpu.overrideCPURegister(8, 25);
		cpu.overrideCPURegister(9, 42);
		memory[0] = 0x01092020; // ADD $4, $8, $9

		makeSnapshot();

		cpu.clock();

		cpuStatusBefore.GPR[4] = 67;
		cpuStatusBefore.PC += 4;

		checkSnapshot();
	}

	TEST_F(CPUALUInstructionsTests, ADDTestWithValidNegativeArguments)
	{
		cpu.overrideCPURegister(8, -25);
		cpu.overrideCPURegister(9, -42);
		memory[0] = 0x01092020; // ADD $4, $8, $9

		makeSnapshot();

		cpu.clock();

		cpuStatusBefore.GPR[4] = -67;
		cpuStatusBefore.PC += 4;

		checkSnapshot();
	}

	TEST_F(CPUALUInstructionsTests, ADDTestWithArgumentsCausingOverflows)
	{
		cpu.overrideCPURegister(8, 0x7FFFFFFF);
		cpu.overrideCPURegister(9, 42);
		memory[0] = 0x01092020; // ADD $4, $8, $9

		makeSnapshot();

		cpu.clock();

		cpuStatusBefore.PC = 0x80000080;
		cop0StatusBefore.CAUSE = 0x30;
		cop0StatusBefore.EPC = 0xBFC00000;

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
