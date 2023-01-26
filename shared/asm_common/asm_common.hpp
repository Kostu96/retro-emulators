#pragma once
#include "../type_aliases.hpp"

#include <sstream>

struct ErrorMessage
{
    u32 line;
    const char* message;
};

void printBytes(std::stringstream& ss, const u8* code, size_t& addr, u8 count, ...);

enum class NumberBase : u16
{
    Bin = 2,
    Oct = 8,
    Dec = 10,
    Hex = 16
};

bool stringToValue(const char* start, size_t length, NumberBase base, u16& value);
u8 digitCharToValue(char c);

bool isDigit(char c);
bool isAlpha(char c);
