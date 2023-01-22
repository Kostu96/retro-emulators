#pragma once
#include "type_aliases.hpp"

#include <iomanip>
#include <sstream>

struct DisassemblyLine
{
    u16 address;
    char buffer[30];
};

void printBytes(std::stringstream& ss, const u8* code, size_t& addr, u8 count, ...);

bool isDigit(char c);
bool isAlpha(char c);
