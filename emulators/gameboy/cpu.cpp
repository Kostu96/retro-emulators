#include "cpu.hpp"

#include <bitset>
#include <cassert>

static const u8 standardCycleCounts[256]{
    1, 3, 2, 2, 1, 1, 2, 1, 5, 2, 2, 2, 1, 1, 2, 1,
    0, 3, 2, 2, 1, 1, 2, 1, 3, 2, 2, 2, 1, 1, 2, 1,
    2, 3, 2, 2, 1, 1, 2, 1, 2, 2, 2, 2, 1, 1, 2, 1,
    2, 3, 2, 2, 3, 3, 3, 1, 2, 2, 2, 2, 1, 1, 2, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    2, 2, 2, 2, 2, 2, 0, 2, 1, 1, 1, 1, 1, 1, 2, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    2, 3, 3, 4, 3, 4, 2, 4, 2, 4, 3, 0, 3, 6, 2, 4,
    2, 3, 3, 0, 3, 4, 2, 4, 2, 4, 3, 0, 3, 0, 2, 4,
    3, 3, 2, 0, 0, 4, 2, 4, 4, 1, 4, 0, 0, 0, 2, 4,
    3, 3, 2, 1, 0, 4, 2, 4, 3, 2, 4, 1, 0, 0, 2, 4
};

static const u8 conditionalCycleCounts[256]{
    1, 3, 2, 2, 1, 1, 2, 1, 5, 2, 2, 2, 1, 1, 2, 1,
    0, 3, 2, 2, 1, 1, 2, 1, 3, 2, 2, 2, 1, 1, 2, 1,
    3, 3, 2, 2, 1, 1, 2, 1, 3, 2, 2, 2, 1, 1, 2, 1,
    3, 3, 2, 2, 3, 3, 3, 1, 3, 2, 2, 2, 1, 1, 2, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    2, 2, 2, 2, 2, 2, 0, 2, 1, 1, 1, 1, 1, 1, 2, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    5, 3, 4, 4, 6, 4, 2, 4, 5, 4, 4, 0, 6, 6, 2, 4,
    5, 3, 4, 0, 6, 4, 2, 4, 5, 4, 4, 0, 6, 0, 2, 4,
    3, 3, 2, 0, 0, 4, 2, 4, 4, 1, 4, 0, 0, 0, 2, 4,
    3, 3, 2, 1, 0, 4, 2, 4, 3, 2, 4, 1, 0, 0, 2, 4
};

static const u8 prefixCycleCounts[256]{
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2,
    2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2,
    2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2,
    2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2
};

void CPU::reset()
{
    m_state.AF = 0;
    m_state.BC = 0;
    m_state.DE = 0;
    m_state.HL = 0;
    m_state.SP = 0xF000;
    m_state.PC = 0;
    m_state.InterruptEnabled = false;
    m_state.IsHalted = false;

    m_prefixMode = false;
    m_conditionalTaken = false;
    m_EIRequested = false;
    m_cyclesLeft = 1;
    m_interruptRequested = false;
}

bool CPU::interrupt(u8 vector)
{
    m_state.IsHalted = false;

    bool ret = m_state.InterruptEnabled;
    if (m_state.InterruptEnabled)
    {
        m_state.InterruptEnabled = false;
        m_interruptRequested = true;
        m_interruptVector = vector;
    }

    return ret;
}

void CPU::clock()
{
    if (m_cyclesLeft > 0)
        m_cyclesLeft--;

    if (m_EIRequested) {
        m_EIRequested = false;
        m_state.InterruptEnabled = true;
    }

    if (m_cyclesLeft == 0)
    {
        if (!m_prefixMode) {
            if (m_interruptRequested) {
                m_interruptRequested = false;
                RST(m_interruptVector);
            }
        }

        if (!m_state.IsHalted) {
            u8 opcode = load8(m_state.PC++);

            if (m_prefixMode) {
                m_prefixMode = false;
                prefixInstruction(opcode);
                m_cyclesLeft += prefixCycleCounts[opcode];
            }
            else {
                standardInstruction(opcode);
                m_cyclesLeft += m_conditionalTaken ? conditionalCycleCounts[opcode] : standardCycleCounts[opcode];
                m_conditionalTaken = false;
            }
        }
    }
}

void CPU::standardInstruction(u8 opcode)
{
    switch (opcode)
    {
    case 0x00: break;
    case 0x01: m_state.BC = load16(m_state.PC); m_state.PC += 2; break;
    case 0x02: store8(m_state.BC, m_state.A); break;
    case 0x03: m_state.BC++; break;
    case 0x04: INCR(m_state.B); break;
    case 0x05: DECR(m_state.B); break;
    case 0x06: m_state.B = load8(m_state.PC++); break;
    case 0x07: RLCA(); break;
    case 0x08: {
        store16(load16(m_state.PC), m_state.SP);
        m_state.PC += 2;
    } break;
    case 0x09: ADDHL(m_state.BC); break;
    case 0x0A: m_state.A = load8(m_state.BC); break;
    case 0x0B: m_state.BC--; break;
    case 0x0C: INCR(m_state.C); break;
    case 0x0D: DECR(m_state.C); break;
    case 0x0E: m_state.C = load8(m_state.PC++); break;
    case 0x0F: RRCA(); break;

    case 0x11: m_state.DE = load16(m_state.PC); m_state.PC += 2;  break;
    case 0x12: store8(m_state.DE, m_state.A); break;
    case 0x13: m_state.DE++; break;
    case 0x14: INCR(m_state.D); break;
    case 0x15: DECR(m_state.D); break;
    case 0x16: m_state.D = load8(m_state.PC++); break;
    case 0x17: RAL(); break;
    case 0x18: JR(true); break;
    case 0x19: ADDHL(m_state.DE); break;
    case 0x1A: m_state.A = load8(m_state.DE); break;
    case 0x1B: m_state.DE--; break;
    case 0x1C: INCR(m_state.E); break;
    case 0x1D: DECR(m_state.E); break;
    case 0x1E: m_state.E = load8(m_state.PC++); break;
    case 0x1F: RAR(); break;
    case 0x20: JR(!m_state.F.Zero); break;
    case 0x21: m_state.HL = load16(m_state.PC); m_state.PC += 2; break;
    case 0x22: store8(m_state.HL++, m_state.A); break;
    case 0x23: m_state.HL++; break;
    case 0x24: INCR(m_state.H); break;
    case 0x25: DECR(m_state.H); break;
    case 0x26: m_state.H = load8(m_state.PC++);  break;
    case 0x27: DAA(); break;
    case 0x28: JR(m_state.F.Zero); break;
    case 0x29: ADDHL(m_state.HL); break;
    case 0x2A: m_state.A = load8(m_state.HL++); break;
    case 0x2B: m_state.HL--; break;
    case 0x2C: INCR(m_state.L); break;
    case 0x2D: DECR(m_state.L); break;
    case 0x2E: m_state.L = load8(m_state.PC++); break;
    case 0x2F: {
        m_state.A = ~m_state.A;
        m_state.F.HalfCarry = 1;
        m_state.F.Subtract = 1;
    } break;
    case 0x30: JR(!m_state.F.Carry); break;
    case 0x31: m_state.SP = load16(m_state.PC); m_state.PC += 2; break;
    case 0x32: store8(m_state.HL--, m_state.A); break;
    case 0x33: m_state.SP++; break;
    case 0x34: INCM(); break;
    case 0x35: DECM(); break;
    case 0x36: store8(m_state.HL, load8(m_state.PC++)); break;
    case 0x37: {
        m_state.F.Carry = 1;
        m_state.F.HalfCarry = 0;
        m_state.F.Subtract = 0;
    } break;
    case 0x38: JR(m_state.F.Carry); break;
    case 0x39: ADDHL(m_state.SP); break;
    case 0x3A: m_state.A = load8(m_state.HL--); break;
    case 0x3B: m_state.SP--; break;
    case 0x3C: INCR(m_state.A); break;
    case 0x3D: DECR(m_state.A); break;
    case 0x3E: m_state.A = load8(m_state.PC++); break;
    case 0x3F: {
        m_state.F.Carry = ~m_state.F.Carry;
        m_state.F.HalfCarry = 0;
        m_state.F.Subtract = 0;
    } break;
    case 0x40: m_state.B = m_state.B; break;
    case 0x41: m_state.B = m_state.C; break;
    case 0x42: m_state.B = m_state.D; break;
    case 0x43: m_state.B = m_state.E; break;
    case 0x44: m_state.B = m_state.H; break;
    case 0x45: m_state.B = m_state.L; break;
    case 0x46: m_state.B = load8(m_state.HL); break;
    case 0x47: m_state.B = m_state.A; break;
    case 0x48: m_state.C = m_state.B; break;
    case 0x49: m_state.C = m_state.C; break;
    case 0x4A: m_state.C = m_state.D; break;
    case 0x4B: m_state.C = m_state.E; break;
    case 0x4C: m_state.C = m_state.H; break;
    case 0x4D: m_state.C = m_state.L; break;
    case 0x4E: m_state.C = load8(m_state.HL); break;
    case 0x4F: m_state.C = m_state.A; break;
    case 0x50: m_state.D = m_state.B; break;
    case 0x51: m_state.D = m_state.C; break;
    case 0x52: m_state.D = m_state.D; break;
    case 0x53: m_state.D = m_state.E; break;
    case 0x54: m_state.D = m_state.H; break;
    case 0x55: m_state.D = m_state.L; break;
    case 0x56: m_state.D = load8(m_state.HL); break;
    case 0x57: m_state.D = m_state.A; break;
    case 0x58: m_state.E = m_state.B; break;
    case 0x59: m_state.E = m_state.C; break;
    case 0x5A: m_state.E = m_state.D; break;
    case 0x5B: m_state.E = m_state.E; break;
    case 0x5C: m_state.E = m_state.H; break;
    case 0x5D: m_state.E = m_state.L; break;
    case 0x5E: m_state.E = load8(m_state.HL); break;
    case 0x5F: m_state.E = m_state.A; break;
    case 0x60: m_state.H = m_state.B; break;
    case 0x61: m_state.H = m_state.C; break;
    case 0x62: m_state.H = m_state.D; break;
    case 0x63: m_state.H = m_state.E; break;
    case 0x64: m_state.H = m_state.H; break;
    case 0x65: m_state.H = m_state.L; break;
    case 0x66: m_state.H = load8(m_state.HL); break;
    case 0x67: m_state.H = m_state.A; break;
    case 0x68: m_state.L = m_state.B; break;
    case 0x69: m_state.L = m_state.C; break;
    case 0x6A: m_state.L = m_state.D; break;
    case 0x6B: m_state.L = m_state.E; break;
    case 0x6C: m_state.L = m_state.H; break;
    case 0x6D: m_state.L = m_state.L; break;
    case 0x6E: m_state.L = load8(m_state.HL); break;
    case 0x6F: m_state.L = m_state.A; break;
    case 0x70: store8(m_state.HL, m_state.B); break;
    case 0x71: store8(m_state.HL, m_state.C); break;
    case 0x72: store8(m_state.HL, m_state.D); break;
    case 0x73: store8(m_state.HL, m_state.E); break;
    case 0x74: store8(m_state.HL, m_state.H); break;
    case 0x75: store8(m_state.HL, m_state.L); break;
    case 0x76: m_state.IsHalted = true; break;
    case 0x77: store8(m_state.HL, m_state.A); break;
    case 0x78: m_state.A = m_state.B; break;
    case 0x79: m_state.A = m_state.C; break;
    case 0x7A: m_state.A = m_state.D; break;
    case 0x7B: m_state.A = m_state.E; break;
    case 0x7C: m_state.A = m_state.H; break;
    case 0x7D: m_state.A = m_state.L; break;
    case 0x7E: m_state.A = load8(m_state.HL); break;
    case 0x7F: m_state.A = m_state.A; break;
    case 0x80: ADD(m_state.B); break;
    case 0x81: ADD(m_state.C); break;
    case 0x82: ADD(m_state.D); break;
    case 0x83: ADD(m_state.E); break;
    case 0x84: ADD(m_state.H); break;
    case 0x85: ADD(m_state.L); break;
    case 0x86: ADD(load8(m_state.HL)); break;
    case 0x87: ADD(m_state.A); break;
    case 0x88: ADC(m_state.B); break;
    case 0x89: ADC(m_state.C); break;
    case 0x8A: ADC(m_state.D); break;
    case 0x8B: ADC(m_state.E); break;
    case 0x8C: ADC(m_state.H); break;
    case 0x8D: ADC(m_state.L); break;
    case 0x8E: ADC(load8(m_state.HL)); break;
    case 0x8F: ADC(m_state.A); break;
    case 0x90: SUB(m_state.B); break;
    case 0x91: SUB(m_state.C); break;
    case 0x92: SUB(m_state.D); break;
    case 0x93: SUB(m_state.E); break;
    case 0x94: SUB(m_state.H); break;
    case 0x95: SUB(m_state.L); break;
    case 0x96: SUB(load8(m_state.HL)); break;
    case 0x97: SUB(m_state.A); break;
    case 0x98: SBB(m_state.B); break;
    case 0x99: SBB(m_state.C); break;
    case 0x9A: SBB(m_state.D); break;
    case 0x9B: SBB(m_state.E); break;
    case 0x9C: SBB(m_state.H); break;
    case 0x9D: SBB(m_state.L); break;
    case 0x9E: SBB(load8(m_state.HL)); break;
    case 0x9F: SBB(m_state.A); break;
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
    case 0xB0: OR(m_state.B); break;
    case 0xB1: OR(m_state.C); break;
    case 0xB2: OR(m_state.D); break;
    case 0xB3: OR(m_state.E); break;
    case 0xB4: OR(m_state.H); break;
    case 0xB5: OR(m_state.L); break;
    case 0xB6: OR(load8(m_state.HL)); break;
    case 0xB7: OR(m_state.A); break;
    case 0xB8: CMP(m_state.B); break;
    case 0xB9: CMP(m_state.C); break;
    case 0xBA: CMP(m_state.D); break;
    case 0xBB: CMP(m_state.E); break;
    case 0xBC: CMP(m_state.H); break;
    case 0xBD: CMP(m_state.L); break;
    case 0xBE: CMP(load8(m_state.HL)); break;
    case 0xBF: CMP(m_state.A); break;
    case 0xC0: RET(!m_state.F.Zero); break;
    case 0xC1: m_state.BC = pop16(); break;
    case 0xC2: JMP(!m_state.F.Zero); break;
    case 0xC3: JMP(true); break;
    case 0xC4: CALL(!m_state.F.Zero); break;
    case 0xC5: push16(m_state.BC); break;
    case 0xC6: ADD(load8(m_state.PC++)); break;
    case 0xC7: RST(0); break;
    case 0xC8: RET(m_state.F.Zero); break;
    case 0xC9: RET(true); break;
    case 0xCA: JMP(m_state.F.Zero); break;
    case 0xCB: m_prefixMode = true; break;
    case 0xCC: CALL(m_state.F.Zero); break;
    case 0xCD: CALL(true); break;
    case 0xCE: ADC(load8(m_state.PC++)); break;
    case 0xCF: RST(1); break;
    case 0xD0: RET(!m_state.F.Carry); break;
    case 0xD1: m_state.DE = pop16(); break;
    case 0xD2: JMP(!m_state.F.Carry); break;
    
    case 0xD4: CALL(!m_state.F.Carry); break;
    case 0xD5: push16(m_state.DE); break;
    case 0xD6: SUB(load8(m_state.PC++)); break;
    case 0xD7: RST(2); break;
    case 0xD8: RET(m_state.F.Carry); break;
    case 0xD9: {
        RET(true);
        m_state.InterruptEnabled = true;
    } break;
    case 0xDA: JMP(m_state.F.Carry); break;
    
    case 0xDC: CALL(m_state.F.Carry); break;

    case 0xDE: SBB(load8(m_state.PC++)); break;
    case 0xDF: RST(3); break;
    case 0xE0: store8(0xFF00 | load8(m_state.PC++), m_state.A); break;
    case 0xE1: m_state.HL = pop16(); break;
    case 0xE2: store8(0xFF00 | m_state.C, m_state.A); break;
    
    case 0xE5: push16(m_state.HL); break;
    case 0xE6: AND(load8(m_state.PC++)); break;
    case 0xE7: RST(4); break;
    case 0xE8: {
        s8 imm = load8(m_state.PC++);
        u16 result8bit = (m_state.SP & 0xFF) + (u8)imm;
        u8 result4bit = (m_state.SP & 0xF) + (imm & 0xF);
        m_state.SP = m_state.SP + imm;
        m_state.F.Carry = result8bit >> 8;
        m_state.F.HalfCarry = result4bit >> 4;
        m_state.F.Subtract = 0;
        m_state.F.Zero = 0;
    } break;
    case 0xE9: m_state.PC = m_state.HL; break;
    case 0xEA: {
        store8(load16(m_state.PC), m_state.A);
        m_state.PC += 2;
    } break;

    case 0xEE: XOR(load8(m_state.PC++)); break;
    case 0xEF: RST(5); break;
    case 0xF0: m_state.A = load8(0xFF00 | load8(m_state.PC++)); break;
    case 0xF1: m_state.AF = pop16();  m_state.F.byte &= 0xF0; break;
    case 0xF2: m_state.A = load8(0xFF00 | m_state.C); break;
    case 0xF3: m_state.InterruptEnabled = false;  break;
    
    case 0xF5: push16(m_state.AF); break;
    case 0xF6: OR(load8(m_state.PC++)); break;
    case 0xF7: RST(6); break;
    case 0xF8: {
        s8 imm = load8(m_state.PC++);
        u16 result8bit = (m_state.SP & 0xFF) + (u8)imm;
        u8 result4bit = (m_state.SP & 0xF) + (imm & 0xF);
        m_state.HL = m_state.SP + imm;
        m_state.F.Carry = result8bit >> 8;
        m_state.F.HalfCarry = result4bit >> 4;
        m_state.F.Subtract = 0;
        m_state.F.Zero = 0;
    } break;
    case 0xF9: m_state.SP = m_state.HL; break;
    case 0xFA: {
        m_state.A = load8(load16(m_state.PC));
        m_state.PC += 2;
    } break;
    case 0xFB: m_EIRequested = true; break;
    
    case 0xFE: CMP(load8(m_state.PC++)); break;
    case 0xFF: RST(7); break;
    default:
        assert(false && "Unhandled standard instruction");
    }
}

void CPU::prefixInstruction(u8 opcode)
{
    switch (opcode)
    {
    case 0x00: RLC(m_state.B); break;
    case 0x01: RLC(m_state.C); break;
    case 0x02: RLC(m_state.D); break;
    case 0x03: RLC(m_state.E); break;
    case 0x04: RLC(m_state.H); break;
    case 0x05: RLC(m_state.L); break;
    case 0x06: RLCM(); break;
    case 0x07: RLC(m_state.A); break;
    case 0x08: RRC(m_state.B); break;
    case 0x09: RRC(m_state.C); break;
    case 0x0A: RRC(m_state.D); break;
    case 0x0B: RRC(m_state.E); break;
    case 0x0C: RRC(m_state.H); break;
    case 0x0D: RRC(m_state.L); break;
    case 0x0E: RRCM(); break;
    case 0x0F: RRC(m_state.A); break;
    case 0x10: RL(m_state.B); break;
    case 0x11: RL(m_state.C); break;
    case 0x12: RL(m_state.D); break;
    case 0x13: RL(m_state.E); break;
    case 0x14: RL(m_state.H); break;
    case 0x15: RL(m_state.L); break;
    case 0x16: RLM(); break;
    case 0x17: RL(m_state.A); break;
    case 0x18: RR(m_state.B); break;
    case 0x19: RR(m_state.C); break;
    case 0x1A: RR(m_state.D); break;
    case 0x1B: RR(m_state.E); break;
    case 0x1C: RR(m_state.H); break;
    case 0x1D: RR(m_state.L); break;
    case 0x1E: RRM(); break;
    case 0x1F: RR(m_state.A); break;
    case 0x20: SLA(m_state.B); break;
    case 0x21: SLA(m_state.C); break;
    case 0x22: SLA(m_state.D); break;
    case 0x23: SLA(m_state.E); break;
    case 0x24: SLA(m_state.H); break;
    case 0x25: SLA(m_state.L); break;
    case 0x26: SLAM(); break;
    case 0x27: SLA(m_state.A); break;
    case 0x28: SRA(m_state.B); break;
    case 0x29: SRA(m_state.C); break;
    case 0x2A: SRA(m_state.D); break;
    case 0x2B: SRA(m_state.E); break;
    case 0x2C: SRA(m_state.H); break;
    case 0x2D: SRA(m_state.L); break;
    case 0x2E: SRAM(); break;
    case 0x2F: SRA(m_state.A); break;
    case 0x30: SWAP(m_state.B); break;
    case 0x31: SWAP(m_state.C); break;
    case 0x32: SWAP(m_state.D); break;
    case 0x33: SWAP(m_state.E); break;
    case 0x34: SWAP(m_state.H); break;
    case 0x35: SWAP(m_state.L); break;
    case 0x36: SWAPM(); break;
    case 0x37: SWAP(m_state.A); break;
    case 0x38: SRL(m_state.B); break;
    case 0x39: SRL(m_state.C); break;
    case 0x3A: SRL(m_state.D); break;
    case 0x3B: SRL(m_state.E); break;
    case 0x3C: SRL(m_state.H); break;
    case 0x3D: SRL(m_state.L); break;
    case 0x3E: SRLM(); break;
    case 0x3F: SRL(m_state.A); break;
    case 0x40: BIT(m_state.B, 0); break;
    case 0x41: BIT(m_state.C, 0); break;
    case 0x42: BIT(m_state.D, 0); break;
    case 0x43: BIT(m_state.E, 0); break;
    case 0x44: BIT(m_state.H, 0); break;
    case 0x45: BIT(m_state.L, 0); break;
    case 0x46: BIT(load8(m_state.HL), 0); break;
    case 0x47: BIT(m_state.A, 0); break;
    case 0x48: BIT(m_state.B, 1); break;
    case 0x49: BIT(m_state.C, 1); break;
    case 0x4A: BIT(m_state.D, 1); break;
    case 0x4B: BIT(m_state.E, 1); break;
    case 0x4C: BIT(m_state.H, 1); break;
    case 0x4D: BIT(m_state.L, 1); break;
    case 0x4E: BIT(load8(m_state.HL), 1); break;
    case 0x4F: BIT(m_state.A, 1); break;
    case 0x50: BIT(m_state.B, 2); break;
    case 0x51: BIT(m_state.C, 2); break;
    case 0x52: BIT(m_state.D, 2); break;
    case 0x53: BIT(m_state.E, 2); break;
    case 0x54: BIT(m_state.H, 2); break;
    case 0x55: BIT(m_state.L, 2); break;
    case 0x56: BIT(load8(m_state.HL), 2); break;
    case 0x57: BIT(m_state.A, 2); break;
    case 0x58: BIT(m_state.B, 3); break;
    case 0x59: BIT(m_state.C, 3); break;
    case 0x5A: BIT(m_state.D, 3); break;
    case 0x5B: BIT(m_state.E, 3); break;
    case 0x5C: BIT(m_state.H, 3); break;
    case 0x5D: BIT(m_state.L, 3); break;
    case 0x5E: BIT(load8(m_state.HL), 3); break;
    case 0x5F: BIT(m_state.A, 3); break;
    case 0x60: BIT(m_state.B, 4); break;
    case 0x61: BIT(m_state.C, 4); break;
    case 0x62: BIT(m_state.D, 4); break;
    case 0x63: BIT(m_state.E, 4); break;
    case 0x64: BIT(m_state.H, 4); break;
    case 0x65: BIT(m_state.L, 4); break;
    case 0x66: BIT(load8(m_state.HL), 4); break;
    case 0x67: BIT(m_state.A, 4); break;
    case 0x68: BIT(m_state.B, 5); break;
    case 0x69: BIT(m_state.C, 5); break;
    case 0x6A: BIT(m_state.D, 5); break;
    case 0x6B: BIT(m_state.E, 5); break;
    case 0x6C: BIT(m_state.H, 5); break;
    case 0x6D: BIT(m_state.L, 5); break;
    case 0x6E: BIT(load8(m_state.HL), 5); break;
    case 0x6F: BIT(m_state.A, 5); break;
    case 0x70: BIT(m_state.B, 6); break;
    case 0x71: BIT(m_state.C, 6); break;
    case 0x72: BIT(m_state.D, 6); break;
    case 0x73: BIT(m_state.E, 6); break;
    case 0x74: BIT(m_state.H, 6); break;
    case 0x75: BIT(m_state.L, 6); break;
    case 0x76: BIT(load8(m_state.HL), 6); break;
    case 0x77: BIT(m_state.A, 6); break;
    case 0x78: BIT(m_state.B, 7); break;
    case 0x79: BIT(m_state.C, 7); break;
    case 0x7A: BIT(m_state.D, 7); break;
    case 0x7B: BIT(m_state.E, 7); break;
    case 0x7C: BIT(m_state.H, 7); break;
    case 0x7D: BIT(m_state.L, 7); break;
    case 0x7E: BIT(load8(m_state.HL), 7); break;
    case 0x7F: BIT(m_state.A, 7); break;
    case 0x80: RES(m_state.B, 0); break;
    case 0x81: RES(m_state.C, 0); break;
    case 0x82: RES(m_state.D, 0); break;
    case 0x83: RES(m_state.E, 0); break;
    case 0x84: RES(m_state.H, 0); break;
    case 0x85: RES(m_state.L, 0); break;
    case 0x86: RESM(0); break;
    case 0x87: RES(m_state.A, 0); break;
    case 0x88: RES(m_state.B, 1); break;
    case 0x89: RES(m_state.C, 1); break;
    case 0x8A: RES(m_state.D, 1); break;
    case 0x8B: RES(m_state.E, 1); break;
    case 0x8C: RES(m_state.H, 1); break;
    case 0x8D: RES(m_state.L, 1); break;
    case 0x8E: RESM(1); break;
    case 0x8F: RES(m_state.A, 1); break;
    case 0x90: RES(m_state.B, 2); break;
    case 0x91: RES(m_state.C, 2); break;
    case 0x92: RES(m_state.D, 2); break;
    case 0x93: RES(m_state.E, 2); break;
    case 0x94: RES(m_state.H, 2); break;
    case 0x95: RES(m_state.L, 2); break;
    case 0x96: RESM(2); break;
    case 0x97: RES(m_state.A, 2); break;
    case 0x98: RES(m_state.B, 3); break;
    case 0x99: RES(m_state.C, 3); break;
    case 0x9A: RES(m_state.D, 3); break;
    case 0x9B: RES(m_state.E, 3); break;
    case 0x9C: RES(m_state.H, 3); break;
    case 0x9D: RES(m_state.L, 3); break;
    case 0x9E: RESM(3); break;
    case 0x9F: RES(m_state.A, 3); break;
    case 0xA0: RES(m_state.B, 4); break;
    case 0xA1: RES(m_state.C, 4); break;
    case 0xA2: RES(m_state.D, 4); break;
    case 0xA3: RES(m_state.E, 4); break;
    case 0xA4: RES(m_state.H, 4); break;
    case 0xA5: RES(m_state.L, 4); break;
    case 0xA6: RESM(4); break;
    case 0xA7: RES(m_state.A, 4); break;
    case 0xA8: RES(m_state.B, 5); break;
    case 0xA9: RES(m_state.C, 5); break;
    case 0xAA: RES(m_state.D, 5); break;
    case 0xAB: RES(m_state.E, 5); break;
    case 0xAC: RES(m_state.H, 5); break;
    case 0xAD: RES(m_state.L, 5); break;
    case 0xAE: RESM(5); break;
    case 0xAF: RES(m_state.A, 5); break;
    case 0xB0: RES(m_state.B, 6); break;
    case 0xB1: RES(m_state.C, 6); break;
    case 0xB2: RES(m_state.D, 6); break;
    case 0xB3: RES(m_state.E, 6); break;
    case 0xB4: RES(m_state.H, 6); break;
    case 0xB5: RES(m_state.L, 6); break;
    case 0xB6: RESM(6); break;
    case 0xB7: RES(m_state.A, 6); break;
    case 0xB8: RES(m_state.B, 7); break;
    case 0xB9: RES(m_state.C, 7); break;
    case 0xBA: RES(m_state.D, 7); break;
    case 0xBB: RES(m_state.E, 7); break;
    case 0xBC: RES(m_state.H, 7); break;
    case 0xBD: RES(m_state.L, 7); break;
    case 0xBE: RESM(7); break;
    case 0xBF: RES(m_state.A, 7); break;
    case 0xC0: SET(m_state.B, 0); break;
    case 0xC1: SET(m_state.C, 0); break;
    case 0xC2: SET(m_state.D, 0); break;
    case 0xC3: SET(m_state.E, 0); break;
    case 0xC4: SET(m_state.H, 0); break;
    case 0xC5: SET(m_state.L, 0); break;
    case 0xC6: SETM(0); break;
    case 0xC7: SET(m_state.A, 0); break;
    case 0xC8: SET(m_state.B, 1); break;
    case 0xC9: SET(m_state.C, 1); break;
    case 0xCA: SET(m_state.D, 1); break;
    case 0xCB: SET(m_state.E, 1); break;
    case 0xCC: SET(m_state.H, 1); break;
    case 0xCD: SET(m_state.L, 1); break;
    case 0xCE: SETM(1); break;
    case 0xCF: SET(m_state.A, 1); break;
    case 0xD0: SET(m_state.B, 2); break;
    case 0xD1: SET(m_state.C, 2); break;
    case 0xD2: SET(m_state.D, 2); break;
    case 0xD3: SET(m_state.E, 2); break;
    case 0xD4: SET(m_state.H, 2); break;
    case 0xD5: SET(m_state.L, 2); break;
    case 0xD6: SETM(2); break;
    case 0xD7: SET(m_state.A, 2); break;
    case 0xD8: SET(m_state.B, 3); break;
    case 0xD9: SET(m_state.C, 3); break;
    case 0xDA: SET(m_state.D, 3); break;
    case 0xDB: SET(m_state.E, 3); break;
    case 0xDC: SET(m_state.H, 3); break;
    case 0xDD: SET(m_state.L, 3); break;
    case 0xDE: SETM(3); break;
    case 0xDF: SET(m_state.A, 3); break;
    case 0xE0: SET(m_state.B, 4); break;
    case 0xE1: SET(m_state.C, 4); break;
    case 0xE2: SET(m_state.D, 4); break;
    case 0xE3: SET(m_state.E, 4); break;
    case 0xE4: SET(m_state.H, 4); break;
    case 0xE5: SET(m_state.L, 4); break;
    case 0xE6: SETM(4); break;
    case 0xE7: SET(m_state.A, 4); break;
    case 0xE8: SET(m_state.B, 5); break;
    case 0xE9: SET(m_state.C, 5); break;
    case 0xEA: SET(m_state.D, 5); break;
    case 0xEB: SET(m_state.E, 5); break;
    case 0xEC: SET(m_state.H, 5); break;
    case 0xED: SET(m_state.L, 5); break;
    case 0xEE: SETM(5); break;
    case 0xEF: SET(m_state.A, 5); break;
    case 0xF0: SET(m_state.B, 6); break;
    case 0xF1: SET(m_state.C, 6); break;
    case 0xF2: SET(m_state.D, 6); break;
    case 0xF3: SET(m_state.E, 6); break;
    case 0xF4: SET(m_state.H, 6); break;
    case 0xF5: SET(m_state.L, 6); break;
    case 0xF6: SETM(6); break;
    case 0xF7: SET(m_state.A, 6); break;
    case 0xF8: SET(m_state.B, 7); break;
    case 0xF9: SET(m_state.C, 7); break;
    case 0xFA: SET(m_state.D, 7); break;
    case 0xFB: SET(m_state.E, 7); break;
    case 0xFC: SET(m_state.H, 7); break;
    case 0xFD: SET(m_state.L, 7); break;
    case 0xFE: SETM(7); break;
    case 0xFF: SET(m_state.A, 7); break;
    default:
        assert(false && "Unhandled prefix instruction");
    }
}

void CPU::XCHG()
{
    u16 temp = m_state.HL;
    m_state.HL = m_state.DE;
    m_state.DE = temp;
}

void CPU::XTHL()
{
    u8 temp = m_state.L;
    m_state.L = load8(m_state.SP);
    store8(m_state.SP, temp);

    temp = m_state.H;
    m_state.H = load8(m_state.SP + 1);
    store8(m_state.SP + 1, temp);
}

void CPU::DECR(u8& reg)
{
    u8 halfResult = (s8)(reg & 0xF) - 1;
    reg--;
    m_state.F.HalfCarry = halfResult >> 4;
    m_state.F.Subtract = 1;
    m_state.F.Zero = (reg == 0);
}

void CPU::DECM()
{
    u8 value = load8(m_state.HL);
    DECR(value);
    store8(m_state.HL, value);
}

void CPU::INCR(u8& reg)
{
    u8 halfResult = (reg & 0xF) + 1;
    reg++;
    m_state.F.HalfCarry = halfResult >> 4;
    m_state.F.Subtract = 0;
    m_state.F.Zero = (reg == 0);
}

void CPU::INCM()
{
    u8 value = load8(m_state.HL);
    INCR(value);
    store8(m_state.HL, value);
}

void CPU::AND(u8 value)
{
    m_state.A &= value;
    m_state.F.Carry = 0;
    m_state.F.HalfCarry = 1;
    m_state.F.Subtract = 0;
    m_state.F.Zero = (m_state.A == 0);
}

void CPU::OR(u8 value)
{
    m_state.A |= value;
    m_state.F.Carry = 0;
    m_state.F.HalfCarry = 0;
    m_state.F.Subtract = 0;
    m_state.F.Zero = (m_state.A == 0);
}

void CPU::XOR(u8 value)
{
    m_state.A ^= value;
    m_state.F.Carry = 0;
    m_state.F.HalfCarry = 0;
    m_state.F.Subtract = 0;
    m_state.F.Zero = (m_state.A == 0);
}

void CPU::JMP(bool flag)
{
    u16 address = load16(m_state.PC);
    m_state.PC += 2;
    if (flag) {
        m_conditionalTaken = true;
        m_state.PC = address;
    }
}

void CPU::JR(bool flag)
{
    s8 offset = load8(m_state.PC++);
    if (flag) {
        m_conditionalTaken = true;
        m_state.PC += offset;
    }
}

void CPU::CALL(bool flag)
{
    u16 address = load16(m_state.PC);
    m_state.PC += 2;
    if (flag) {
        m_conditionalTaken = true;
        push16(m_state.PC);
        m_state.PC = address;
    }
}

void CPU::RET(bool flag)
{
    if (flag) {
        m_conditionalTaken = true;
        m_state.PC = pop16();
    }
}

void CPU::RST(u8 vector)
{
    push16(m_state.PC);
    m_state.PC = vector * 8;
}

void CPU::ADD(u8 value)
{
    u16 result = m_state.A + value;
    u8 result4bit = (m_state.A & 0xF) + (value & 0xF);
    m_state.A = result & 0xFF;
    m_state.F.Carry = result >> 8;
    m_state.F.HalfCarry = result4bit >> 4;
    m_state.F.Subtract = 0;
    m_state.F.Zero = (m_state.A == 0);
}

void CPU::ADC(u8 value)
{
    u16 result = m_state.A + value + m_state.F.Carry;
    u8 result4bit = (m_state.A & 0xF) + (value & 0xF) + m_state.F.Carry;
    m_state.A = result & 0xFF;
    m_state.F.Carry = result >> 8;
    m_state.F.HalfCarry = result4bit >> 4;
    m_state.F.Subtract = 0;
    m_state.F.Zero = (m_state.A == 0);
}

void CPU::ADDHL(u16 value)
{
    u32 result = m_state.HL + value;
    u16 result12bit = (m_state.HL & 0xFFF) + (value & 0xFFF);
    m_state.HL = result & 0xFFFF;
    m_state.F.Carry = result >> 16;
    m_state.F.HalfCarry = result12bit >> 12;
    m_state.F.Subtract = 0;
}

void CPU::SUB(u8 value)
{
    u16 result = (s16)m_state.A - (s16)value;
    u8 result4bit = (s8)(m_state.A & 0xF) - (s8)(value & 0xF);
    m_state.A = result & 0xFF;
    m_state.F.Carry = result >> 8;
    m_state.F.HalfCarry = result4bit >> 4;
    m_state.F.Subtract = 1;
    m_state.F.Zero = (m_state.A == 0);
}

void CPU::SBB(u8 value)
{
    u16 result = (s16)m_state.A - (s16)value - m_state.F.Carry;
    u8 result4bit = (s8)(m_state.A & 0xF) - (s8)(value & 0xF) - m_state.F.Carry;
    m_state.A = result & 0xFF;
    m_state.F.Carry = result >> 8;
    m_state.F.HalfCarry = result4bit >> 4;
    m_state.F.Subtract = 1;
    m_state.F.Zero = (m_state.A == 0);
}

void CPU::CMP(u8 value)
{
    u16 result = (s16)m_state.A - (s16)value;
    u8 result4bit = (s8)(m_state.A & 0xF) - (s8)(value & 0xF);
    m_state.F.Carry = result >> 8;
    m_state.F.HalfCarry = result4bit >> 4;
    m_state.F.Subtract = 1;
    m_state.F.Zero = (result == 0);
}

void CPU::DAA()
{
    u8 u = 0;
    u8 fc = 0;

    if (m_state.F.HalfCarry || (!m_state.F.Subtract && (m_state.A & 0xF) > 9)) {
        u = 6;
    }
    if (m_state.F.Carry || (!m_state.F.Subtract && m_state.A > 0x99)) {
        u |= 0x60;
        fc = 1;
    }

    m_state.A += m_state.F.Subtract ? -u : u;
    m_state.F.Carry = fc;
    m_state.F.HalfCarry = 0;
    m_state.F.Zero = (m_state.A == 0);
}

void CPU::RLCA()
{
    // rotate left
    u8 newCarry = m_state.A >> 7;
    m_state.A <<= 1;
    m_state.A |= newCarry;
    m_state.F.Carry = newCarry;
    m_state.F.HalfCarry = 0;
    m_state.F.Subtract = 0;
    m_state.F.Zero = 0;
}

void CPU::RAL()
{
    // rotate left with carry
    u8 newCarry = m_state.A >> 7;
    m_state.A <<= 1;
    m_state.A |= m_state.F.Carry;
    m_state.F.Carry = newCarry;
    m_state.F.HalfCarry = 0;
    m_state.F.Subtract = 0;
    m_state.F.Zero = 0;
}

void CPU::RRCA()
{
    // rotate right
    u8 newCarry = m_state.A & 1;
    m_state.A >>= 1;
    m_state.A |= newCarry << 7;
    m_state.F.Carry = newCarry;
    m_state.F.HalfCarry = 0;
    m_state.F.Subtract = 0;
    m_state.F.Zero = 0;
}

void CPU::RAR()
{
    // rotate right with carry
    u8 newCarry = m_state.A & 1;
    m_state.A >>= 1;
    m_state.A |= m_state.F.Carry << 7;
    m_state.F.Carry = newCarry;
    m_state.F.HalfCarry = 0;
    m_state.F.Subtract = 0;
    m_state.F.Zero = 0;
}

void CPU::RLC(u8& reg)
{
    // rotate left
    u8 newCarry = reg >> 7;
    reg <<= 1;
    reg |= newCarry;
    m_state.F.Carry = newCarry;
    m_state.F.HalfCarry = 0;
    m_state.F.Subtract = 0;
    m_state.F.Zero = (reg == 0);
}

void CPU::RLCM()
{
    u8 value = load8(m_state.HL);
    RLC(value);
    store8(m_state.HL, value);
}

void CPU::RL(u8& reg)
{
    // rotate left with carry
    u8 newCarry = reg >> 7;
    reg <<= 1;
    reg |= m_state.F.Carry;
    m_state.F.Carry = newCarry;
    m_state.F.HalfCarry = 0;
    m_state.F.Subtract = 0;
    m_state.F.Zero = (reg == 0);
}

void CPU::RLM()
{
    u8 value = load8(m_state.HL);
    RL(value);
    store8(m_state.HL, value);
}

void CPU::RRC(u8& reg)
{
    // rotate right
    u8 newCarry = reg & 1;
    reg >>= 1;
    reg |= newCarry << 7;
    m_state.F.Carry = newCarry;
    m_state.F.HalfCarry = 0;
    m_state.F.Subtract = 0;
    m_state.F.Zero = (reg == 0);
}

void CPU::RRCM()
{
    u8 value = load8(m_state.HL);
    RRC(value);
    store8(m_state.HL, value);
}

void CPU::RR(u8& reg)
{
    // rotate right with carry
    u8 newCarry = reg & 1;
    reg >>= 1;
    reg |= m_state.F.Carry << 7;
    m_state.F.Carry = newCarry;
    m_state.F.HalfCarry = 0;
    m_state.F.Subtract = 0;
    m_state.F.Zero = (reg == 0);
}

void CPU::RRM()
{
    u8 value = load8(m_state.HL);
    RR(value);
    store8(m_state.HL, value);
}

void CPU::SLA(u8& reg)
{
    // shift left
    u8 newCarry = reg >> 7;
    reg <<= 1;
    m_state.F.Carry = newCarry;
    m_state.F.HalfCarry = 0;
    m_state.F.Subtract = 0;
    m_state.F.Zero = (reg == 0);
}

void CPU::SLAM()
{
    u8 value = load8(m_state.HL);
    SLA(value);
    store8(m_state.HL, value);
}

void CPU::SRA(u8& reg)
{
    // shift right arithmetic
    u8 newCarry = reg & 1;
    reg >>= 1;
    reg |= (reg << 1) & 0x80;
    m_state.F.Carry = newCarry;
    m_state.F.HalfCarry = 0;
    m_state.F.Subtract = 0;
    m_state.F.Zero = (reg == 0);
}

void CPU::SRAM()
{
    u8 value = load8(m_state.HL);
    SRA(value);
    store8(m_state.HL, value);
}

void CPU::SRL(u8& reg)
{
    // shift right logic
    u8 newCarry = reg & 1;
    reg >>= 1;
    m_state.F.Carry = newCarry;
    m_state.F.HalfCarry = 0;
    m_state.F.Subtract = 0;
    m_state.F.Zero = (reg == 0);
}

void CPU::SRLM()
{
    u8 value = load8(m_state.HL);
    SRL(value);
    store8(m_state.HL, value);
}

void CPU::SWAP(u8& reg)
{
    u8 l = reg & 0x0F;
    u8 h = reg & 0xF0;
    reg = (l << 4) | (h >> 4);
    m_state.F.Carry = 0;
    m_state.F.HalfCarry = 0;
    m_state.F.Subtract = 0;
    m_state.F.Zero = (reg == 0);
}

void CPU::SWAPM()
{
    u8 value = load8(m_state.HL);
    SWAP(value);
    store8(m_state.HL, value);
}

void CPU::BIT(u8 value, u8 bit)
{
    m_state.F.HalfCarry = 1;
    m_state.F.Subtract = 0;
    m_state.F.Zero = !((value >> bit) & 1);
}

void CPU::RES(u8& reg, u8 bit)
{
    u8 mask = ~(1 << bit);
    reg &= mask;
}

void CPU::RESM(u8 bit)
{
    u8 value = load8(m_state.HL);
    RES(value, bit);
    store8(m_state.HL, value);
}

void CPU::SET(u8& reg, u8 bit)
{
    u8 mask = 1 << bit;
    reg |= mask;
}

void CPU::SETM(u8 bit)
{
    u8 value = load8(m_state.HL);
    SET(value, bit);
    store8(m_state.HL, value);
}
