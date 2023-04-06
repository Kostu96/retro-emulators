#include "cpu_operations.hpp"

void add8(u8 lhs, u8 rhs, u8 carry, u16& result, u8& result4bit)
{
    result = lhs + rhs + carry;
    result4bit = (lhs & 0xF) + (rhs & 0xF) + carry;
}
