#pragma once
#include <cstdint>

struct AddressRange
{
    uint32_t start;
    uint32_t size;

    bool contains(uint32_t address, uint32_t& offset) const
    {
        offset = address - start;
        return (address >= start) && (address < start + size);
    }
};

struct RegisterIndex
{
    uint32_t index;

    RegisterIndex(uint32_t pIndex) : index(pIndex) {}
};

struct Instruction
{
    uint32_t word;

    Instruction(uint32_t pWord) : word(pWord) {}

    uint32_t opcode() const { return word >> 26; }
    uint32_t subfn() const { return word & 0x3F; }
    uint32_t copfn() const { return (word >> 21) & 0x1F; }
    RegisterIndex regS() const { return (word >> 21) & 0x1F; }
    RegisterIndex regT() const { return (word >> 16) & 0x1F; }
    RegisterIndex regD() const { return (word >> 11) & 0x1F; }
    uint32_t imm() const { return word & 0xFFFF; }
    uint32_t imm_se() const { return static_cast<int16_t>(word & 0xFFFF); }
    uint32_t imm_jump() const { return word & 0x3FFFFFF; }
    uint32_t shift() const { return (word >> 6) & 0x1F; }
};
