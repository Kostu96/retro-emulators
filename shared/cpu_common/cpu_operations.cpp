#include "cpu_operations.hpp"

void add8(u8 lhs, u8 rhs, u8 carry, u16& result, u8& result4bit)
{
    result = lhs + rhs + carry;
    result4bit = (lhs & 0xF) + (rhs & 0xF) + carry;
}

void add16(u16 lhs, u16 rhs, u32& result, u16& result12bit)
{
    result = lhs + rhs;
    result12bit = (lhs & 0xFFF) + (rhs & 0xFFF);
}

void sub8(u8 lhs, u8 rhs, u8 borrow, u16& result, u8& result4bit)
{
    result = (s16)lhs - (s16)rhs - borrow;
    result4bit = (s8)(lhs & 0xF) - (s8)(rhs & 0xF) - borrow;
}

void rotateLeft(u8& reg, u8& newCarry)
{
    newCarry = reg >> 7;
    reg <<= 1;
    reg |= newCarry;
}

void rotateLeftWithCarry(u8& reg, u8 carry, u8& newCarry)
{
    newCarry = reg >> 7;
    reg <<= 1;
    reg |= carry;
}

void rotateRight(u8& reg, u8& newCarry)
{
    newCarry = reg & 1;
    reg >>= 1;
    reg |= newCarry << 7;
}

void rotateRightWithCarry(u8& reg, u8 carry, u8& newCarry)
{
    newCarry = reg & 1;
    reg >>= 1;
    reg |= carry << 7;
}

void shiftLeft(u8& reg, u8& newCarry)
{
    newCarry = reg >> 7;
    reg <<= 1;
}

void shiftRightArithmetic(u8& reg, u8& newCarry)
{
    newCarry = reg & 1;
    reg >>= 1;
    reg |= (reg << 1) & 0x80;
}

void shiftRightLogic(u8& reg, u8& newCarry)
{
    newCarry = reg & 1;
    reg >>= 1;
}
