#pragma once
#include "utils/types.hpp"

#include <sstream>

struct ErrorMessage
{
    u32 line;
    const char* message;
};

void printBytes(std::ostringstream& ss, const u8* code, size_t& addr, u8 count, ...);
