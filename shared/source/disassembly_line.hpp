#pragma once
#include "types.hpp"

struct DisassemblyLine
{
    u16 address;
    char buffer[30];
};
