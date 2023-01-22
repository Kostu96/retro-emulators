#pragma once
#include "../../shared/asm_common.hpp"

#include <vector>

void disassemble(const u8* code, size_t code_size, std::vector<DisassemblyLine>& output);
