#pragma once
#include "cpu.hpp"
#include "utils/disassembly_line.hpp"

namespace PSX {

    void disasm(u32 address, CPUInstruction inst, DisassemblyLine& output);

} // namespace PSX
