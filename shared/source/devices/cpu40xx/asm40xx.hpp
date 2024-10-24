#pragma once
#include "shared/source/asm/asm_common.hpp"

#include <vector>

struct DisassemblyLine;

namespace ASM40xx
{

	bool assemble(const char* source, std::vector<u8>& output, std::vector<ErrorMessage>& errors);
	void disassemble(const u8* code, size_t code_size, std::vector<DisassemblyLine>& output);

}
