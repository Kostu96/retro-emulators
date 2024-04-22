#pragma once
#include "cpu.hpp"

void initGBDoctor();
void shutdownGBDoctor();
void updateGBDoctor(bool isBootloaderUnmapped, bool isLastCycle, const CPU::State& cpuState, CPU::ReadMemoryCallback readMemory);
