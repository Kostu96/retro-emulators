#pragma once
#include "shared/source/types.hpp"

#include <vector>

struct DisassemblyLine;

void disassemble(const u8* code, size_t code_size, std::vector<DisassemblyLine>& output);
