#pragma once
#include <ccl/types.hpp>

void add8(u8 lhs, u8 rhs, u8 carry, u16& result, u8& result4bit);
void add16(u16 lhs, u16 rhs, u32& result, u16& result12bit);
void sub8(u8 lhs, u8 rhs, u8 borrow, u16& result, u8& result4bit);

void rotateLeft(u8& reg, u8& newCarry);
void rotateLeftWithCarry(u8& reg, u8 carry, u8& newCarry);
void rotateRight(u8& reg, u8& newCarry);
void rotateRightWithCarry(u8& reg, u8 carry, u8& newCarry);
void shiftLeft(u8& reg, u8& newCarry);
void shiftRightArithmetic(u8& reg, u8& newCarry);
void shiftRightLogic(u8& reg, u8& newCarry);
