#include "disasm8008.hpp"

#include <cassert>

static void printBytes(std::stringstream& ss, const u8* code, size_t& addr, u8 count, ...)
{
    std::va_list args;
    va_start(args, count);
    for (u8 i = 0; i < count; ++i)
    {
        u8* byte = va_arg(args, u8*);
        *byte = code[addr++];
        ss << std::hex << std::setw(2) << (u16)*byte << ' ';
    }
    va_end(args);
}

#define PRINT1 printBytes(ss, code, addr, 1, &byte1)
#define PRINT2 printBytes(ss, code, addr, 2, &byte1, &byte2)
#define INST1(str) ss << "       " str; break
#define INST2(str) PRINT1; ss << "    " str << std::setw(2) << (u16)byte1; break
#define INST3(str) PRINT2; ss << " " str << std::setw(2) << (u16)byte2 << std::setw(2) << (u16)byte1; break

void disassemble(const u8* code, size_t code_size, std::vector<DisassemblyLine>& output)
{
    for (size_t addr = 0; addr < code_size; )
    {
        DisassemblyLine line;
        line.address = addr;
        std::stringstream ss;
        ss << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << addr << ":  ";
        u8 byte = code[addr++];
        ss << std::hex << std::setw(2) << (u16)byte << ' ';

        u8 byte1, byte2;
        switch (byte)
        {
        
        default: INST1("???");
        }

        assert(ss.str().size() <= sizeof(line.buffer));
        strcpy_s(line.buffer, ss.str().c_str());
        output.push_back(line);
    }
}
