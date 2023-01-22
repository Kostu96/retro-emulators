#pragma once
#include "asm_common.hpp"

#include <vector>

namespace ASM8008
{

	void assemble(const char* source, std::vector<u8>& output);
	void disassemble(const u8* code, size_t code_size, std::vector<DisassemblyLine>& output);

}
