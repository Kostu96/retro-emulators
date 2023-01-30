#include "cpu8080.hpp"

#include <bitset>
#include <cassert>

void CPU8080::reset()
{
    AF = 0;
    BC = 0;
    DE = 0;
    HL = 0;
    SP = 0xF000;
    PC = 0;
}

void CPU8080::clock()
{
    u8 instruction = load8(PC++);

    switch (instruction)
    {
    case 0x00: break;
    case 0x01: LDRP(BC, load16(PC)); PC += 2; break;

    case 0x05: DECR(B); break;
    case 0x06: LDR(B, load8(PC++)); break;

    case 0x0A: LDR(A, load8(BC)); break;

    case 0x0D: DECR(C); break;
    case 0x0E: LDR(C, load8(PC++)); break;
    case 0x0F: RRC(); break;

    case 0x11: LDRP(DE, load16(PC)); PC += 2; break;

    case 0x13: INCRP(DE); break;
    case 0x14: INCR(D); break;
    case 0x15: DECR(D); break;

    case 0x19: ADDHL(DE); break;
    case 0x1A: LDR(A, load8(DE)); break;

    case 0x1E: LDR(E, load8(PC++)); break;

    case 0x21: LDRP(HL, load16(PC)); PC += 2; break;

    case 0x23: INCRP(HL); break;

    case 0x25: DECR(H); break;
    case 0x26: LDR(H, load8(PC++)); break;

    case 0x29: ADDHL(HL); break;

    case 0x31: LDRP(SP, load16(PC)); PC += 2; break;
    case 0x32: LDM(load16(PC), A); PC += 2; break;

    case 0x36: LDM(HL, load8(PC++)); break;

    case 0x39: ADDHL(SP); break;
    case 0x3A: LDR(A, load8(load16(PC))); PC += 2; break;

    case 0x3E: LDR(A, load8(PC++)); break;

    case 0x40: LDR(B, B); break;
    case 0x41: LDR(B, C); break;
    case 0x42: LDR(B, D); break;
    case 0x43: LDR(B, E); break;
    case 0x44: LDR(B, H); break;
    case 0x45: LDR(B, L); break;
    case 0x46: LDR(B, load8(HL)); break;
    case 0x47: LDR(B, A); break;
    case 0x48: LDR(C, B); break;
    case 0x49: LDR(C, C); break;
    case 0x4A: LDR(C, D); break;
    case 0x4B: LDR(C, E); break;
    case 0x4C: LDR(C, H); break;
    case 0x4D: LDR(C, L); break;
    case 0x4E: LDR(C, load8(HL)); break;
    case 0x4F: LDR(C, A); break;
    case 0x50: LDR(D, B); break;
    case 0x51: LDR(D, C); break;
    case 0x52: LDR(D, D); break;
    case 0x53: LDR(D, E); break;
    case 0x54: LDR(D, H); break;
    case 0x55: LDR(D, L); break;
    case 0x56: LDR(D, load8(HL)); break;
    case 0x57: LDR(D, A); break;
    case 0x58: LDR(E, B); break;
    case 0x59: LDR(E, C); break;
    case 0x5A: LDR(E, D); break;
    case 0x5B: LDR(E, E); break;
    case 0x5C: LDR(E, H); break;
    case 0x5D: LDR(E, L); break;
    case 0x5E: LDR(E, load8(HL)); break;
    case 0x5F: LDR(E, A); break;
    case 0x60: LDR(H, B); break;
    case 0x61: LDR(H, C); break;
    case 0x62: LDR(H, D); break;
    case 0x63: LDR(H, E); break;
    case 0x64: LDR(H, H); break;
    case 0x65: LDR(H, L); break;
    case 0x66: LDR(H, load8(HL)); break;
    case 0x67: LDR(H, A); break;
    case 0x68: LDR(L, B); break;
    case 0x69: LDR(L, C); break;
    case 0x6A: LDR(L, D); break;
    case 0x6B: LDR(L, E); break;
    case 0x6C: LDR(L, H); break;
    case 0x6D: LDR(L, L); break;
    case 0x6E: LDR(L, load8(HL)); break;
    case 0x6F: LDR(L, A); break;
    case 0x70: LDM(HL, B); break;
    case 0x71: LDM(HL, C); break;
    case 0x72: LDM(HL, D); break;
    case 0x73: LDM(HL, E); break;
    case 0x74: LDM(HL, H); break;
    case 0x75: LDM(HL, L); break;

    case 0x77: LDM(HL, A); break;
    case 0x78: LDR(A, B); break;
    case 0x79: LDR(A, C); break;
    case 0x7A: LDR(A, D); break;
    case 0x7B: LDR(A, E); break;
    case 0x7C: LDR(A, H); break;
    case 0x7D: LDR(A, L); break;
    case 0x7E: LDR(A, load8(HL)); break;
    case 0x7F: LDR(A, A); break;
    case 0x80: ADD(B); break;
    case 0x81: ADD(C); break;
    case 0x82: ADD(D); break;
    case 0x83: ADD(E); break;
    case 0x84: ADD(H); break;
    case 0x85: ADD(L); break;
    case 0x86: ADD(load8(HL)); break;
    case 0x87: ADD(A); break;

    case 0xA0: AND(B); break;
    case 0xA1: AND(C); break;
    case 0xA2: AND(D); break;
    case 0xA3: AND(E); break;
    case 0xA4: AND(H); break;
    case 0xA5: AND(L); break;
    case 0xA6: AND(load8(HL)); break;
    case 0xA7: AND(A); break;
    case 0xA8: XOR(B); break;
    case 0xA9: XOR(C); break;
    case 0xAA: XOR(D); break;
    case 0xAB: XOR(E); break;
    case 0xAC: XOR(H); break;
    case 0xAD: XOR(L); break;
    case 0xAE: XOR(load8(HL)); break;
    case 0xAF: XOR(A); break;

    case 0xB8: CMP(B); break;
    case 0xB9: CMP(C); break;
    case 0xBA: CMP(D); break;
    case 0xBB: CMP(E); break;
    case 0xBC: CMP(H); break;
    case 0xBD: CMP(L); break;
    case 0xBE: CMP(load8(HL)); break;
    case 0xBF: CMP(A); break;

    case 0xC0: RET(!F.Z); break;

    case 0xC2: JMP(!F.Z); break;
    case 0xC3: JMP(true); break;

    case 0xC6: ADD(load8(PC++)); break;

    case 0xC9: RET(true); break;

    case 0xCD: CALL(true); break;

    case 0xD0: RET(!F.C); break;
    case 0xD1: DE = pop16(); break;

    case 0xD3: /* to be implemented */ break;

    case 0xD5: push16(DE); break;

    case 0xE1: HL = pop16(); break;

    case 0xE5: push16(HL); break;
    case 0xE6: AND(load8(PC++)); break;

    case 0xEB: XCH(); break;

    case 0xF1: AF = pop16(); break;

    case 0xF5: push16(AF); break;

    case 0xFE: CMP(load8(PC++)); break;
    default:
        assert(false && "Unhandled instruction");
    }
}

#pragma region MemoryAccess
u8 CPU8080::load8(u16 address) const
{
    u8 data = 0;
    bool read = false;
    for (auto& entry : m_readMap)
    {
        u16 offset;
        if (entry.range.contains(address, offset))
        {
            data = entry.read(offset);
            read = true;
            break;
        }
    }
    assert(read && "Unhandled memory read");
    return data;
}

u16 CPU8080::load16(u16 address)
{
    u16 data = 0;
    bool read = false;
    for (auto& entry : m_readMap)
    {
        u16 offset;
        if (entry.range.contains(address, offset))
        {
            data = entry.read(offset + 1);
            data <<= 8;
            data |= entry.read(offset);
            read = true;
            break;
        }
    }
    assert(read && "Unhandled memory read");
    return data;
}

void CPU8080::store8(u16 address, u8 data)
{
    bool stored = false;
    for (auto& entry : m_writeMap)
    {
        u16 offset;
        if (entry.range.contains(address, offset))
        {
            entry.write(offset, data);
            stored = true;
            break;
        }
    }
    assert(stored && "Unhandled memory write");
}

void CPU8080::store16(u16 address, u16 data)
{
    bool stored = false;
    for (auto& entry : m_writeMap)
    {
        u16 offset;
        if (entry.range.contains(address, offset))
        {
            entry.write(offset, data & 0xFF);
            entry.write(offset + 1, data >> 8);
            stored = true;
            break;
        }
    }
    assert(stored && "Unhandled memory write");
}

void CPU8080::push8(u8 data)
{
    store8(--SP, data);
}

void CPU8080::push16(u16 data)
{
    store16(SP - 2, data);
    SP -= 2;
}

u8 CPU8080::pop8()
{
    return load8(SP++);
}

u16 CPU8080::pop16()
{
    SP += 2;
    return load16(SP - 2);
}
#pragma endregion

void CPU8080::LDR(u8& dst, u8 value)
{
    dst = value;
}

void CPU8080::LDRP(u16& dst, u16 value)
{
    dst = value;
}

void CPU8080::LDM(u16 address, u8 value)
{
    store8(address, value);
}

void CPU8080::XCH()
{
    u16 temp = HL;
    HL = DE;
    DE = temp;
}

void CPU8080::ADD(u8 value)
{
    u16 result = A + value;
    u8 result4bit = (A & 0xF) + (value & 0xF);
    A = result;
    F.Z = (A == 0);
    F.S = result >> 7;
    F.AC = result4bit >> 4;
    F.C = result >> 8;
    F.P = (std::bitset<8>(A).count() % 2) == 0;
}

void CPU8080::ADDHL(u16 value)
{
    u32 result = HL + value;
    u16 result12bit = (HL & 0xFFF) + (value & 0xFFF);
    HL = result;
    F.Z = (HL == 0);
    F.S = 0;
    F.AC = result12bit >> 12;
    F.C = result >> 16;
}

void CPU8080::CMP(u8 value)
{
    u16 result = (s16)A - (s16)value;
    u8 result4bit = (s8)A - (s8)value;
    F.Z = (result == 0);
    F.S = 1;
    F.AC = (result4bit >> 4) & 1;
    F.C = (result >> 8) & 1;
}

void CPU8080::DECR(u8& reg)
{
    u8 tempBit = ~reg & 0x10;
    reg--;
    F.AC = ((reg & 0x10) & tempBit) >> 4;
    F.S = 1;
    F.Z = reg == 0;
    F.P = (std::bitset<8>(reg).count() % 2) == 0;
}

void CPU8080::DECRP(u16& reg)
{
    reg--;
}

void CPU8080::INCR(u8& reg)
{
    u8 tempBit = reg & 0x10;
    reg++;
    F.AC = (~(reg & 0x10) & tempBit) >> 4;
    F.S = 0;
    F.Z = reg == 0;
    F.P = (std::bitset<8>(reg).count() % 2) == 0;
}

void CPU8080::INCRP(u16& reg)
{
    reg++;
}

void CPU8080::RRC()
{
    F.byte = 0;
    F.C = A & 1;
    A >>= 1;
    A |= F.C;
}

void CPU8080::AND(u8 value)
{
    A &= value;
    F.byte = 0;
    F.Z = A == 0;
    F.AC = 1;
}

void CPU8080::XOR(u8 value)
{
    A ^= value;
    F.byte = 0x0F;
    F.Z = A == 0;
}

void CPU8080::JMP(bool flag)
{
    u16 address = load16(PC);
    PC += 2;
    if (flag) {
        PC = address;
    }
}

void CPU8080::CALL(bool flag)
{
    u16 address = load16(PC);
    PC += 2;
    if (flag) {
        push16(PC);
        PC = address;
    }
}

void CPU8080::RET(bool flag)
{
    if (flag) {
        PC = pop16();
    }
}
