#pragma once
#include "type_aliases.hpp"

#include <cstdarg>
#include <iomanip>
#include <sstream>

struct DisassemblyLine
{
    u16 address;
    char buffer[30];
};
