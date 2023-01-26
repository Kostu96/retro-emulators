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

bool stringToValue(const char* start, size_t length, NumberBase base, u16& value)
{
    value = 0;
    u16 pos = 1;
    for (int i = length - 1; i >= 0; i--)
    {
        switch (base)
        {
        case NumberBase::Bin:
            if (start[i] < '0' || start[i] > '1')
                return false;
        case NumberBase::Oct:
            if (start[i] < '0' || start[i] > '7')
                return false;
        case NumberBase::Dec:
            if (start[i] < '0' || start[i] > '9')
                return false;
        case NumberBase::Hex:
            if (start[i] < '0' || start[i] > '9')
                return false;
            if (start[i] < 'a' || start[i] > 'f')
                return false;
            if (start[i] < 'A' || start[i] > 'F')
                return false;
        }

        value += digitCharToValue(start[i]) * pos;
        pos *= static_cast<u16>(base);
    }

    return true;
}

u8 digitCharToValue(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    else if (c >= 'A' && c <= 'F') return 10 + c - 'A';
    else if (c >= 'a' && c <= 'f') return 10 + c - 'a';

    return 0xFF;
}

bool isDigit(char c)
{
    return (c >= '0' && c <= '9');
}

bool isAlpha(char c)
{
    return (c >= 'A' && c <= 'Z');
}
