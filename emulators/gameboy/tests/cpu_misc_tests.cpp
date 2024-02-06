#include "cpu_tests_fixture.hpp"

using CPUMiscTests = CPUTests;

TEST_F(CPUMiscTests, NOPTest)
{
	rom[0x0] = 0x00; // NOP

	auto preExecutionState = captureCPUState();
	cpu.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
}
