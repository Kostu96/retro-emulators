#include "asm_common.hpp"

#include <cstdarg>
#include <iomanip>

void printBytes(std::stringstream& ss, const u8* code, size_t& addr, u8 count, ...)
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

u8 digitCharToValue(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    else if (c >= 'A' && c <= 'F') return 10 + c - 'A';
    else if (c >= 'a' && c <= 'f') return 10 + c - 'a';

    return 0xFF;
}
