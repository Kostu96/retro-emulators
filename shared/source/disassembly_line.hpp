#pragma once
#include "types.hpp"

struct DisassemblyLine
{
    static constexpr size_t BUFFER_SIZE = 28;

    u32 address;
    char buffer[BUFFER_SIZE];
};
