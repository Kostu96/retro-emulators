#include "cpu8080.hpp"

#include <bitset>
#include <cassert>

void CPU8080::reset()
{
    m_state.AF = 0;
    m_state.BC = 0;
    m_state.DE = 0;
    m_state.HL = 0;
    m_state.SP = 0xF000;
    m_state.PC = 0;
}

void CPU8080::clock()
{
    u8 instruction = load8(m_state.PC++);

    switch (instruction)
    {
    case 0x00: break;
    case 0x01: LDRP(m_state.BC, load16(m_state.PC)); m_state.PC += 2; break;

    case 0x05: DECR(m_state.B); break;
    case 0x06: LDR(m_state.B, load8(m_state.PC++)); break;

    case 0x0A: LDR(m_state.A, load8(m_state.BC)); break;

    case 0x0D: DECR(m_state.C); break;
    case 0x0E: LDR(m_state.C, load8(m_state.PC++)); break;
    case 0x0F: RRC(); break;

    case 0x11: LDRP(m_state.DE, load16(m_state.PC)); m_state.PC += 2; break;

    case 0x13: INCRP(m_state.DE); break;
    case 0x14: INCR(m_state.D); break;
    case 0x15: DECR(m_state.D); break;

    case 0x19: ADDHL(m_state.DE); break;
    case 0x1A: LDR(m_state.A, load8(m_state.DE)); break;

    case 0x1E: LDR(m_state.E, load8(m_state.PC++)); break;

    case 0x21: LDRP(m_state.HL, load16(m_state.PC)); m_state.PC += 2; break;

    case 0x23: INCRP(m_state.HL); break;

    case 0x25: DECR(m_state.H); break;
    case 0x26: LDR(m_state.H, load8(m_state.PC++)); break;

    case 0x29: ADDHL(m_state.HL); break;

    case 0x31: LDRP(m_state.SP, load16(m_state.PC)); m_state.PC += 2; break;
    case 0x32: LDM(load16(m_state.PC), m_state.A); m_state.PC += 2; break;

    case 0x36: LDM(m_state.HL, load8(m_state.PC++)); break;

    case 0x39: ADDHL(m_state.SP); break;
    case 0x3A: LDR(m_state.A, load8(load16(m_state.PC))); m_state.PC += 2; break;

    case 0x3E: LDR(m_state.A, load8(m_state.PC++)); break;

    case 0x40: LDR(m_state.B, m_state.B); break;
    case 0x41: LDR(m_state.B, m_state.C); break;
    case 0x42: LDR(m_state.B, m_state.D); break;
    case 0x43: LDR(m_state.B, m_state.E); break;
    case 0x44: LDR(m_state.B, m_state.H); break;
    case 0x45: LDR(m_state.B, m_state.L); break;
    case 0x46: LDR(m_state.B, load8(m_state.HL)); break;
    case 0x47: LDR(m_state.B, m_state.A); break;
    case 0x48: LDR(m_state.C, m_state.B); break;
    case 0x49: LDR(m_state.C, m_state.C); break;
    case 0x4A: LDR(m_state.C, m_state.D); break;
    case 0x4B: LDR(m_state.C, m_state.E); break;
    case 0x4C: LDR(m_state.C, m_state.H); break;
    case 0x4D: LDR(m_state.C, m_state.L); break;
    case 0x4E: LDR(m_state.C, load8(m_state.HL)); break;
    case 0x4F: LDR(m_state.C, m_state.A); break;
    case 0x50: LDR(m_state.D, m_state.B); break;
    case 0x51: LDR(m_state.D, m_state.C); break;
    case 0x52: LDR(m_state.D, m_state.D); break;
    case 0x53: LDR(m_state.D, m_state.E); break;
    case 0x54: LDR(m_state.D, m_state.H); break;
    case 0x55: LDR(m_state.D, m_state.L); break;
    case 0x56: LDR(m_state.D, load8(m_state.HL)); break;
    case 0x57: LDR(m_state.D, m_state.A); break;
    case 0x58: LDR(m_state.E, m_state.B); break;
    case 0x59: LDR(m_state.E, m_state.C); break;
    case 0x5A: LDR(m_state.E, m_state.D); break;
    case 0x5B: LDR(m_state.E, m_state.E); break;
    case 0x5C: LDR(m_state.E, m_state.H); break;
    case 0x5D: LDR(m_state.E, m_state.L); break;
    case 0x5E: LDR(m_state.E, load8(m_state.HL)); break;
    case 0x5F: LDR(m_state.E, m_state.A); break;
    case 0x60: LDR(m_state.H, m_state.B); break;
    case 0x61: LDR(m_state.H, m_state.C); break;
    case 0x62: LDR(m_state.H, m_state.D); break;
    case 0x63: LDR(m_state.H, m_state.E); break;
    case 0x64: LDR(m_state.H, m_state.H); break;
    case 0x65: LDR(m_state.H, m_state.L); break;
    case 0x66: LDR(m_state.H, load8(m_state.HL)); break;
    case 0x67: LDR(m_state.H, m_state.A); break;
    case 0x68: LDR(m_state.L, m_state.B); break;
    case 0x69: LDR(m_state.L, m_state.C); break;
    case 0x6A: LDR(m_state.L, m_state.D); break;
    case 0x6B: LDR(m_state.L, m_state.E); break;
    case 0x6C: LDR(m_state.L, m_state.H); break;
    case 0x6D: LDR(m_state.L, m_state.L); break;
    case 0x6E: LDR(m_state.L, load8(m_state.HL)); break;
    case 0x6F: LDR(m_state.L, m_state.A); break;
    case 0x70: LDM(m_state.HL, m_state.B); break;
    case 0x71: LDM(m_state.HL, m_state.C); break;
    case 0x72: LDM(m_state.HL, m_state.D); break;
    case 0x73: LDM(m_state.HL, m_state.E); break;
    case 0x74: LDM(m_state.HL, m_state.H); break;
    case 0x75: LDM(m_state.HL, m_state.L); break;

    case 0x77: LDM(m_state.HL, m_state.A); break;
    case 0x78: LDR(m_state.A, m_state.B); break;
    case 0x79: LDR(m_state.A, m_state.C); break;
    case 0x7A: LDR(m_state.A, m_state.D); break;
    case 0x7B: LDR(m_state.A, m_state.E); break;
    case 0x7C: LDR(m_state.A, m_state.H); break;
    case 0x7D: LDR(m_state.A, m_state.L); break;
    case 0x7E: LDR(m_state.A, load8(m_state.HL)); break;
    case 0x7F: LDR(m_state.A, m_state.A); break;
    case 0x80: ADD(m_state.B); break;
    case 0x81: ADD(m_state.C); break;
    case 0x82: ADD(m_state.D); break;
    case 0x83: ADD(m_state.E); break;
    case 0x84: ADD(m_state.H); break;
    case 0x85: ADD(m_state.L); break;
    case 0x86: ADD(load8(m_state.HL)); break;
    case 0x87: ADD(m_state.A); break;

    case 0xA0: AND(m_state.B); break;
    case 0xA1: AND(m_state.C); break;
    case 0xA2: AND(m_state.D); break;
    case 0xA3: AND(m_state.E); break;
    case 0xA4: AND(m_state.H); break;
    case 0xA5: AND(m_state.L); break;
    case 0xA6: AND(load8(m_state.HL)); break;
    case 0xA7: AND(m_state.A); break;
    case 0xA8: XOR(m_state.B); break;
    case 0xA9: XOR(m_state.C); break;
    case 0xAA: XOR(m_state.D); break;
    case 0xAB: XOR(m_state.E); break;
    case 0xAC: XOR(m_state.H); break;
    case 0xAD: XOR(m_state.L); break;
    case 0xAE: XOR(load8(m_state.HL)); break;
    case 0xAF: XOR(m_state.A); break;

    case 0xB8: CMP(m_state.B); break;
    case 0xB9: CMP(m_state.C); break;
    case 0xBA: CMP(m_state.D); break;
    case 0xBB: CMP(m_state.E); break;
    case 0xBC: CMP(m_state.H); break;
    case 0xBD: CMP(m_state.L); break;
    case 0xBE: CMP(load8(m_state.HL)); break;
    case 0xBF: CMP(m_state.A); break;

    case 0xC0: RET(!m_state.F.Z); break;

    case 0xC2: JMP(!m_state.F.Z); break;
    case 0xC3: JMP(true); break;

    case 0xC6: ADD(load8(m_state.PC++)); break;

    case 0xC9: RET(true); break;

    case 0xCD: CALL(true); break;

    case 0xD0: RET(!m_state.F.C); break;
    case 0xD1: m_state.DE = pop16(); break;

    case 0xD3: /* to be implemented */ break;

    case 0xD5: push16(m_state.DE); break;

    case 0xE1: m_state.HL = pop16(); break;

    case 0xE5: push16(m_state.HL); break;
    case 0xE6: AND(load8(m_state.PC++)); break;

    case 0xEB: XCH(); break;

    case 0xF1: m_state.AF = pop16(); break;

    case 0xF5: push16(m_state.AF); break;

    case 0xFE: CMP(load8(m_state.PC++)); break;
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
    store8(--m_state.SP, data);
}

void CPU8080::push16(u16 data)
{
    store16(m_state.SP - 2, data);
    m_state.SP -= 2;
}

u8 CPU8080::pop8()
{
    return load8(m_state.SP++);
}

u16 CPU8080::pop16()
{
    m_state.SP += 2;
    return load16(m_state.SP - 2);
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
    u16 temp = m_state.HL;
    m_state.HL = m_state.DE;
    m_state.DE = temp;
}

void CPU8080::ADD(u8 value)
{
    u16 result = m_state.A + value;
    u8 result4bit = (m_state.A & 0xF) + (value & 0xF);
    m_state.A = result;
    m_state.F.Z = (m_state.A == 0);
    m_state.F.S = result >> 7;
    m_state.F.AC = result4bit >> 4;
    m_state.F.C = result >> 8;
    m_state.F.P = (std::bitset<8>(m_state.A).count() % 2) == 0;
}

void CPU8080::ADDHL(u16 value)
{
    u32 result = m_state.HL + value;
    u16 result12bit = (m_state.HL & 0xFFF) + (value & 0xFFF);
    m_state.HL = result;
    m_state.F.Z = (m_state.HL == 0);
    m_state.F.S = 0;
    m_state.F.AC = result12bit >> 12;
    m_state.F.C = result >> 16;
}

void CPU8080::CMP(u8 value)
{
    u16 result = (s16)m_state.A - (s16)value;
    u8 result4bit = (s8)m_state.A - (s8)value;
    m_state.F.Z = (result == 0);
    m_state.F.S = 1;
    m_state.F.AC = (result4bit >> 4) & 1;
    m_state.F.C = (result >> 8) & 1;
}

void CPU8080::DECR(u8& reg)
{
    u8 tempBit = ~reg & 0x10;
    reg--;
    m_state.F.AC = ((reg & 0x10) & tempBit) >> 4;
    m_state.F.S = 1;
    m_state.F.Z = reg == 0;
    m_state.F.P = (std::bitset<8>(reg).count() % 2) == 0;
}

void CPU8080::DECRP(u16& reg)
{
    reg--;
}

void CPU8080::INCR(u8& reg)
{
    u8 tempBit = reg & 0x10;
    reg++;
    m_state.F.AC = (~(reg & 0x10) & tempBit) >> 4;
    m_state.F.S = 0;
    m_state.F.Z = reg == 0;
    m_state.F.P = (std::bitset<8>(reg).count() % 2) == 0;
}

void CPU8080::INCRP(u16& reg)
{
    reg++;
}

void CPU8080::RRC()
{
    m_state.F.byte = 0;
    m_state.F.C = m_state.A & 1;
    m_state.A >>= 1;
    m_state.A |= m_state.F.C;
}

void CPU8080::AND(u8 value)
{
    m_state.A &= value;
    m_state.F.byte = 0;
    m_state.F.Z = m_state.A == 0;
    m_state.F.AC = 1;
}

void CPU8080::XOR(u8 value)
{
    m_state.A ^= value;
    m_state.F.byte = 0x0F;
    m_state.F.Z = m_state.A == 0;
}

void CPU8080::JMP(bool flag)
{
    u16 address = load16(m_state.PC);
    m_state.PC += 2;
    if (flag) {
        m_state.PC = address;
    }
}

void CPU8080::CALL(bool flag)
{
    u16 address = load16(m_state.PC);
    m_state.PC += 2;
    if (flag) {
        push16(m_state.PC);
        m_state.PC = address;
    }
}

void CPU8080::RET(bool flag)
{
    if (flag) {
        m_state.PC = pop16();
    }
}
