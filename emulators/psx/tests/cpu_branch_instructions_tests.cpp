#include "cpu_instructions_tests_fixture.hpp"

namespace PSX {

    using CPUBranchInstructionsTests = CPUInstructionsTests;

	TEST_F(CPUBranchInstructionsTests, JTest)
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

	TEST_F(CPUBranchInstructionsTests, BNETestWithRegistersEqual)
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

	TEST_F(CPUBranchInstructionsTests, BNETestWithRegistersNotEqualAndPositiveOffset)
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

	TEST_F(CPUBranchInstructionsTests, BNETestWithRegistersNotEqualAndNegativeOffset)
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
