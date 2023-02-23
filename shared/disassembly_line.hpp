#pragma once
#include <ccl/types.hpp>

struct DisassemblyLine
{
    u16 address;
    char buffer[30];
};
