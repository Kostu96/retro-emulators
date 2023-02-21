#include "cpu_x80_gameboy_mode_fixture.hpp"

using CPUx80GameBoyModeMiscTests = CPUx80GameBoyModeTests;

TEST_F(CPUx80GameBoyModeMiscTests, NOPTest)
{
	rom[0x0] = 0x00; // NOP

	auto preExecutionState = captureCPUState();
	CPU.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
}
