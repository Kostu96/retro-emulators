#pragma once
#include "cpu.hpp"
#include "shared/source/disassembly_line.hpp"

namespace PSX {

    void disasm(u32 address, CPU::Instruction inst, DisassemblyLine& output);

} // namespace PSX
