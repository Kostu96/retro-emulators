#include "cpu8080_gameboy_mode_fixture.hpp"

using CPU8080GameBoyModeMiscTests = CPU8080GameBoyModeTests;

TEST_F(CPU8080GameBoyModeMiscTests, NOPTest)
{
	rom[0x0] = 0x00; // NOP

	auto preExecutionState = captureCPUState();
	CPU.clock();
	auto postExecutionState = captureCPUState();

	preExecutionState.PC = 0x0001;
	compareCPUStates(preExecutionState, postExecutionState);
}
