#include "utils/asm_common.hpp"

#include <cstdarg>
#include <iomanip>

void printBytes(std::ostringstream& ss, const u8* code, size_t& addr, u8 count, ...)
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
