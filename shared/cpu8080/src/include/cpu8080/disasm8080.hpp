#pragma once
#include "utils/asm_common.hpp"

#include <vector>

struct DisassemblyLine;

void disassemble(const u8* code, size_t code_size, std::vector<DisassemblyLine>& output);

void disasmIntruction(u8 opcode, u8 byte1, u8 byte2, DisassemblyLine& output);
