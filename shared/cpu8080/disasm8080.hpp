#pragma once
#include "../type_aliases.hpp"

#include <vector>

void disassemble(const u8* code, size_t code_size, std::vector<DisassemblyLine>& output);
