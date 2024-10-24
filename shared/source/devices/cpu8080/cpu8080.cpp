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
    m_interruptEnabled = false;
    m_isHalted = false;

    m_conditionalTaken = false;
    m_EIRequested = false;
    m_cyclesLeft = 0;
    m_interruptRequested = false;
}

bool CPU8080::interrupt(u8 vector)
{
    m_isHalted = false;

    bool ret = m_interruptEnabled;
    if (m_interruptEnabled)
    {
        m_interruptEnabled = false;
        m_interruptRequested = true;
        m_interruptVector = vector;
    }

    return ret;
}

void CPU8080::clock()
{
    m_cyclesLeft = 0; // TODO: temp WA until i8080 cycle times are implemented

    if (m_cyclesLeft > 0)
        m_cyclesLeft--;

    if (m_EIRequested) {
        m_EIRequested = false;
        m_interruptEnabled = true;
    }

    if (m_cyclesLeft == 0)
    {
        if (m_interruptRequested) {
            m_interruptRequested = false;
            RST(m_interruptVector);
        }

        if (!m_isHalted) {
            u8 opcode = load8(m_state.PC++);

            executeInstruction(opcode);
            // m_cyclesLeft += m_conditionalTaken ? conditionalCycleCounts[opcode] : standardCycleCounts[opcode];
            m_conditionalTaken = false;
        }
    }
}

void CPU8080::executeInstruction(u8 opcode)
{
    switch (opcode)
    {
    case 0x00:                                   break;
    case 0x01: m_state.BC = load16(m_state.PC); m_state.PC += 2;         break;
    case 0x02: store8(m_state.BC, m_state.A);                    break;
    case 0x03: m_state.BC++;                             break;
    case 0x04: INCR(m_state.B);                          break;
    case 0x05: DECR(m_state.B);                          break;
    case 0x06: m_state.B = load8(m_state.PC++);                  break;
    case 0x07: RLC();                            break;
                                                 
    case 0x09: ADDHL(m_state.BC);                        break;
    case 0x0A: m_state.A = load8(m_state.BC);                    break;
    case 0x0B: m_state.BC--;                             break;
    case 0x0C: INCR(m_state.C);                          break;
    case 0x0D: DECR(m_state.C);                          break;
    case 0x0E: m_state.C = load8(m_state.PC++);                  break;
    case 0x0F: RRC();                            break;
                                                 
    case 0x11: m_state.DE = load16(m_state.PC); m_state.PC += 2;         break;
    case 0x12: store8(m_state.DE, m_state.A);                    break;
    case 0x13: m_state.DE++;                             break;
    case 0x14: INCR(m_state.D);                          break;
    case 0x15: DECR(m_state.D);                          break;
    case 0x16: m_state.D = load8(m_state.PC++);                  break;
    case 0x17: RAL();                            break;
                                                 
    case 0x19: ADDHL(m_state.DE);                        break;
    case 0x1A: m_state.A = load8(m_state.DE);                    break;
    case 0x1B: m_state.DE--;                             break;
    case 0x1C: INCR(m_state.E);                          break;
    case 0x1D: DECR(m_state.E);                          break;
    case 0x1E: m_state.E = load8(m_state.PC++);                  break;
    case 0x1F: RAR();                            break;
    
    case 0x21: m_state.HL = load16(m_state.PC); m_state.PC += 2;         break;
    case 0x22: store16(load16(m_state.PC), m_state.HL); m_state.PC += 2; break;
    case 0x23: m_state.HL++;                             break;
    case 0x24: INCR(m_state.H);                          break;
    case 0x25: DECR(m_state.H);                          break;
    case 0x26: m_state.H = load8(m_state.PC++);                  break;
    case 0x27: DAA();                            break;

    case 0x29: ADDHL(m_state.HL);                        break;
    case 0x2A: m_state.HL = load16(load16(m_state.PC)); m_state.PC += 2; break;
    case 0x2B: m_state.HL--;                             break;
    case 0x2C: INCR(m_state.L);                          break;
    case 0x2D: DECR(m_state.L);                          break;
    case 0x2E: m_state.L = load8(m_state.PC++);                  break;
    case 0x2F: m_state.A = ~m_state.A;                           break;

    case 0x31: m_state.SP = load16(m_state.PC); m_state.PC += 2;         break;
    case 0x32: store8(load16(m_state.PC), m_state.A); m_state.PC += 2;   break;
    case 0x33: m_state.SP++;                             break;
    case 0x34: INCM();                           break;
    case 0x35: DECM();                           break;
    case 0x36: store8(m_state.HL, load8(m_state.PC++));          break;
    case 0x37: m_state.F.Carry = 1;                      break;

    case 0x39: ADDHL(m_state.SP);                        break;
    case 0x3A: m_state.A = load8(load16(m_state.PC)); m_state.PC += 2;   break;
    case 0x3B: m_state.SP--;                             break;
    case 0x3C: INCR(m_state.A);                          break;
    case 0x3D: DECR(m_state.A);                          break;
    case 0x3E: m_state.A = load8(m_state.PC++);                  break;
    case 0x3F: m_state.F.Carry = ~m_state.F.Carry;               break;
    case 0x40: m_state.B = m_state.B;                            break;
    case 0x41: m_state.B = m_state.C;                            break;
    case 0x42: m_state.B = m_state.D;                            break;
    case 0x43: m_state.B = m_state.E;                            break;
    case 0x44: m_state.B = m_state.H;                            break;
    case 0x45: m_state.B = m_state.L;                            break;
    case 0x46: m_state.B = load8(m_state.HL);                    break;
    case 0x47: m_state.B = m_state.A;                            break;
    case 0x48: m_state.C = m_state.B;                            break;
    case 0x49: m_state.C = m_state.C;                            break;
    case 0x4A: m_state.C = m_state.D;                            break;
    case 0x4B: m_state.C = m_state.E;                            break;
    case 0x4C: m_state.C = m_state.H;                            break;
    case 0x4D: m_state.C = m_state.L;                            break;
    case 0x4E: m_state.C = load8(m_state.HL);                    break;
    case 0x4F: m_state.C = m_state.A;                            break;
    case 0x50: m_state.D = m_state.B;                            break;
    case 0x51: m_state.D = m_state.C;                            break;
    case 0x52: m_state.D = m_state.D;                            break;
    case 0x53: m_state.D = m_state.E;                            break;
    case 0x54: m_state.D = m_state.H;                            break;
    case 0x55: m_state.D = m_state.L;                            break;
    case 0x56: m_state.D = load8(m_state.HL);                    break;
    case 0x57: m_state.D = m_state.A;                            break;
    case 0x58: m_state.E = m_state.B;                            break;
    case 0x59: m_state.E = m_state.C;                            break;
    case 0x5A: m_state.E = m_state.D;                            break;
    case 0x5B: m_state.E = m_state.E;                            break;
    case 0x5C: m_state.E = m_state.H;                            break;
    case 0x5D: m_state.E = m_state.L;                            break;
    case 0x5E: m_state.E = load8(m_state.HL);                    break;
    case 0x5F: m_state.E = m_state.A;                            break;
    case 0x60: m_state.H = m_state.B;                            break;
    case 0x61: m_state.H = m_state.C;                            break;
    case 0x62: m_state.H = m_state.D;                            break;
    case 0x63: m_state.H = m_state.E;                            break;
    case 0x64: m_state.H = m_state.H;                            break;
    case 0x65: m_state.H = m_state.L;                            break;
    case 0x66: m_state.H = load8(m_state.HL);                    break;
    case 0x67: m_state.H = m_state.A;                            break;
    case 0x68: m_state.L = m_state.B;                            break;
    case 0x69: m_state.L = m_state.C;                            break;
    case 0x6A: m_state.L = m_state.D;                            break;
    case 0x6B: m_state.L = m_state.E;                            break;
    case 0x6C: m_state.L = m_state.H;                            break;
    case 0x6D: m_state.L = m_state.L;                            break;
    case 0x6E: m_state.L = load8(m_state.HL);                    break;
    case 0x6F: m_state.L = m_state.A;                            break;
    case 0x70: store8(m_state.HL, m_state.B);                    break;
    case 0x71: store8(m_state.HL, m_state.C);                    break;
    case 0x72: store8(m_state.HL, m_state.D);                    break;
    case 0x73: store8(m_state.HL, m_state.E);                    break;
    case 0x74: store8(m_state.HL, m_state.H);                    break;
    case 0x75: store8(m_state.HL, m_state.L);                    break;
    case 0x76: m_isHalted = true;                break;
    case 0x77: store8(m_state.HL, m_state.A);                    break;
    case 0x78: m_state.A = m_state.B;                            break;
    case 0x79: m_state.A = m_state.C;                            break;
    case 0x7A: m_state.A = m_state.D;                            break;
    case 0x7B: m_state.A = m_state.E;                            break;
    case 0x7C: m_state.A = m_state.H;                            break;
    case 0x7D: m_state.A = m_state.L;                            break;
    case 0x7E: m_state.A = load8(m_state.HL);                    break;
    case 0x7F: m_state.A = m_state.A;                            break;
    case 0x80: ADD(m_state.B);                           break;
    case 0x81: ADD(m_state.C);                           break;
    case 0x82: ADD(m_state.D);                           break;
    case 0x83: ADD(m_state.E);                           break;
    case 0x84: ADD(m_state.H);                           break;
    case 0x85: ADD(m_state.L);                           break;
    case 0x86: ADD(load8(m_state.HL));                   break;
    case 0x87: ADD(m_state.A);                           break;
    case 0x88: ADC(m_state.B);                           break;
    case 0x89: ADC(m_state.C);                           break;
    case 0x8A: ADC(m_state.D);                           break;
    case 0x8B: ADC(m_state.E);                           break;
    case 0x8C: ADC(m_state.H);                           break;
    case 0x8D: ADC(m_state.L);                           break;
    case 0x8E: ADC(load8(m_state.HL));                   break;
    case 0x8F: ADC(m_state.A);                           break;
    case 0x90: SUB(m_state.B);                           break;
    case 0x91: SUB(m_state.C);                           break;
    case 0x92: SUB(m_state.D);                           break;
    case 0x93: SUB(m_state.E);                           break;
    case 0x94: SUB(m_state.H);                           break;
    case 0x95: SUB(m_state.L);                           break;
    case 0x96: SUB(load8(m_state.HL));                   break;
    case 0x97: SUB(m_state.A);                           break;
    case 0x98: SBB(m_state.B);                           break;
    case 0x99: SBB(m_state.C);                           break;
    case 0x9A: SBB(m_state.D);                           break;
    case 0x9B: SBB(m_state.E);                           break;
    case 0x9C: SBB(m_state.H);                           break;
    case 0x9D: SBB(m_state.L);                           break;
    case 0x9E: SBB(load8(m_state.HL));                   break;
    case 0x9F: SBB(m_state.A);                           break;
    case 0xA0: AND(m_state.B);                           break;
    case 0xA1: AND(m_state.C);                           break;
    case 0xA2: AND(m_state.D);                           break;
    case 0xA3: AND(m_state.E);                           break;
    case 0xA4: AND(m_state.H);                           break;
    case 0xA5: AND(m_state.L);                           break;
    case 0xA6: AND(load8(m_state.HL));                   break;
    case 0xA7: AND(m_state.A);                           break;
    case 0xA8: XOR(m_state.B);                           break;
    case 0xA9: XOR(m_state.C);                           break;
    case 0xAA: XOR(m_state.D);                           break;
    case 0xAB: XOR(m_state.E);                           break;
    case 0xAC: XOR(m_state.H);                           break;
    case 0xAD: XOR(m_state.L);                           break;
    case 0xAE: XOR(load8(m_state.HL));                   break;
    case 0xAF: XOR(m_state.A);                           break;
    case 0xB0: OR(m_state.B);                            break;
    case 0xB1: OR(m_state.C);                            break;
    case 0xB2: OR(m_state.D);                            break;
    case 0xB3: OR(m_state.E);                            break;
    case 0xB4: OR(m_state.H);                            break;
    case 0xB5: OR(m_state.L);                            break;
    case 0xB6: OR(load8(m_state.HL));                    break;
    case 0xB7: OR(m_state.A);                            break;
    case 0xB8: CMP(m_state.B);                           break;
    case 0xB9: CMP(m_state.C);                           break;
    case 0xBA: CMP(m_state.D);                           break;
    case 0xBB: CMP(m_state.E);                           break;
    case 0xBC: CMP(m_state.H);                           break;
    case 0xBD: CMP(m_state.L);                           break;
    case 0xBE: CMP(load8(m_state.HL));                   break;
    case 0xBF: CMP(m_state.A);                           break;
    case 0xC0: RET(!m_state.F.Carry);                    break;
    case 0xC1: m_state.BC = pop16();                     break;
    case 0xC2: JMP(!m_state.F.Zero);                     break;
    case 0xC3: JMP(true);                        break;
    case 0xC4: CALL(!m_state.F.Zero);                    break;
    case 0xC5: push16(m_state.BC);                       break;
    case 0xC6: ADD(load8(m_state.PC++));                 break;
    case 0xC7: RST(0);                           break;
    case 0xC8: RET(m_state.F.Zero);                      break;
    case 0xC9: RET(true);                        break;
    case 0xCA: JMP(m_state.F.Zero);                      break;

    case 0xCC: CALL(m_state.F.Zero);                     break;
    case 0xCD: CALL(true);                       break;
    case 0xCE: ADC(load8(m_state.PC++));                 break;
    case 0xCF: RST(1);                           break;
    case 0xD0: RET(!m_state.F.Carry);                    break;
    case 0xD1: m_state.DE = pop16();                     break;
    case 0xD2: JMP(!m_state.F.Carry);                    break;
    case 0xD3: load8(m_state.PC++);                      break; /* to be implemented */
    case 0xD4: CALL(!m_state.F.Carry);                   break;
    case 0xD5: push16(m_state.DE);                       break;
    case 0xD6: SUB(load8(m_state.PC++));                 break;
    case 0xD7: RST(2);                           break;
    case 0xD8: RET(m_state.F.Carry);                     break;

    case 0xDA: JMP(m_state.F.Carry);                     break;
    case 0xDB: load8(m_state.PC++);                      break; /* to be implemented */
    case 0xDC: CALL(m_state.F.Carry);                    break;

    case 0xDE: SBB(load8(m_state.PC++));                 break;
    case 0xDF: RST(3);                           break;
    case 0xE0: RET(!m_state.F.Parity);                   break;
    case 0xE1: m_state.HL = pop16();                     break;
    case 0xE2: JMP(!m_state.F.Parity);                   break;
    case 0xE3: XTHL();                           break;
    case 0xE4: CALL(!m_state.F.Parity);                  break;
    case 0xE5: push16(m_state.HL);                       break;
    case 0xE6: AND(load8(m_state.PC++));                 break;
    case 0xE7: RST(4);                           break;
    case 0xE8: RET(m_state.F.Parity);                    break;
    case 0xE9: m_state.PC = m_state.HL;                          break;
    case 0xEA: JMP(m_state.F.Parity);                    break;
    case 0xEB: XCHG();                           break;
    case 0xEC: CALL(m_state.F.Parity);                   break;

    case 0xEE: XOR(load8(m_state.PC++));                 break;
    case 0xEF: RST(5);                           break;
    case 0xF0: RET(!m_state.F.Sign);                     break;
    case 0xF1: m_state.AF = pop16(); m_state.F.byte &= 0xF0;     break;
    case 0xF2: JMP(!m_state.F.Sign);                     break;
    case 0xF3: m_interruptEnabled = false;       break;
    case 0xF4: CALL(!m_state.F.Sign);                    break;
    case 0xF5: push16(m_state.AF);                       break;
    case 0xF6: OR(load8(m_state.PC++));                  break;
    case 0xF7: RST(6);                           break;
    case 0xF8: RET(m_state.F.Sign);                      break;
    case 0xF9: m_state.SP = m_state.HL;                          break;
    case 0xFA: JMP(m_state.F.Sign);                      break;
    case 0xFB: m_EIRequested = true;             break;
    case 0xFC: CALL(m_state.F.Sign);                     break;

    case 0xFE: CMP(load8(m_state.PC++));                 break;
    case 0xFF: RST(7);                           break;
    default:
        assert(false && "Unhandled opcode!");
    }
}

void CPU8080::ADD(u8 value)
{
    u16 result = m_state.A + value;
    u8 result4bit = (m_state.A & 0xF) + (value & 0xF);
    m_state.A = result & 0xFF;

    m_state.F.Zero = (m_state.A == 0);
    m_state.F.HalfCarry = (result4bit >> 4);
    m_state.F.Carry = (result >> 8);
    m_state.F.Sign = (result >> 7);
    m_state.F.Parity = ((std::bitset<8>(m_state.A).count() % 2) == 0);
}

void CPU8080::ADC(u8 value)
{
    u16 result = m_state.A + value + m_state.F.Carry;
    u8 result4bit = (m_state.A & 0xF) + (value & 0xF) + m_state.F.Carry;
    m_state.A = result & 0xFF;

    m_state.F.Zero = (m_state.A == 0);
    m_state.F.HalfCarry = (result4bit >> 4);
    m_state.F.Carry = (result >> 8);
    m_state.F.Sign = (result >> 7);
    m_state.F.Parity = ((std::bitset<8>(m_state.A).count() % 2) == 0);
}

void CPU8080::ADDHL(u16 value)
{
    u32 result = m_state.HL + value;
    //u16 result12bit = (HL & 0xFFF) + (value & 0xFFF); TODO: check this
    m_state.HL = result & 0xFFFF;

    m_state.F.Carry = (result >> 16);
}

void CPU8080::AND(u8 value)
{
    m_state.A &= value;

    m_state.F.Zero = (m_state.A == 0);
    m_state.F.Parity = ((std::bitset<8>(m_state.A).count() % 2) == 0);
}

void CPU8080::CALL(bool flag)
{
    u16 address = load16(m_state.PC);
    m_state.PC += 2;
    if (flag) {
        m_conditionalTaken = true;
        push16(m_state.PC);
        m_state.PC = address;
    }
}

void CPU8080::CMP(u8 value)
{
    u16 result = (s16)m_state.A - (s16)value;
    u8 result4bit = (s8)(m_state.A & 0xF) - (s8)(value & 0xF);

    m_state.F.Zero = (result == 0);
    m_state.F.HalfCarry = (result4bit >> 4);
    m_state.F.Carry = (result >> 8);
    m_state.F.Sign = (result >> 7);
    m_state.F.Parity = ((std::bitset<8>(m_state.A).count() % 2) == 0);
}

void CPU8080::DAA()
{
    u16 value = m_state.A;

    if ((value & 0x0F) > 9 || m_state.F.HalfCarry)
        value += 0x06;

    if ((value >> 4) > 9 || m_state.F.Carry)
        value += 0x60;

    m_state.A = value & 0xFF;

    m_state.F.Carry = (value >> 8);
    m_state.F.Parity = ((std::bitset<8>(m_state.A).count() % 2) == 0);
    m_state.F.Sign = (m_state.A >> 7);
    m_state.F.Zero = (m_state.A == 0);
}

void CPU8080::DECM()
{
    u8 value = load8(m_state.HL);
    DECR(value);
    store8(m_state.HL, value);
}

void CPU8080::DECR(u8& reg)
{
    u8 halfResult = (s8)(reg & 0xF) - 1;
    reg--;

    m_state.F.HalfCarry = (halfResult >> 4);
    m_state.F.Zero = (reg == 0);
    m_state.F.Sign = (reg >> 7);
    m_state.F.Parity = ((std::bitset<8>(reg).count() % 2) == 0);
}

void CPU8080::INCM()
{
    u8 value = load8(m_state.HL);
    INCR(value);
    store8(m_state.HL, value);
}

void CPU8080::INCR(u8& reg)
{
    u8 halfResult = (reg & 0xF) + 1;
    reg++;

    m_state.F.HalfCarry = (halfResult >> 4);
    m_state.F.Zero = (reg == 0);
    m_state.F.Sign = (reg >> 7);
    m_state.F.Parity = ((std::bitset<8>(reg).count() % 2) == 0);
}

void CPU8080::JMP(bool flag)
{
    u16 address = load16(m_state.PC);
    m_state.PC += 2;
    if (flag) {
        m_conditionalTaken = true;
        m_state.PC = address;
    }
}

void CPU8080::OR(u8 value)
{
    m_state.A |= value;

    m_state.F.Zero = (m_state.A == 0);
    m_state.F.Parity = ((std::bitset<8>(m_state.A).count() % 2) == 0);
}

void CPU8080::RAL()
{
    // rotate left with carry
    u8 newCarry = m_state.A >> 7;
    m_state.A <<= 1;
    m_state.A |= m_state.F.Carry;

    m_state.F.Carry = newCarry;
}

void CPU8080::RAR()
{
    // rotate right with carry
    u8 newCarry = m_state.A & 1;
    m_state.A >>= 1;
    m_state.A |= m_state.F.Carry << 7;

    m_state.F.Carry = newCarry;
}

void CPU8080::RET(bool flag)
{
    if (flag) {
        m_conditionalTaken = true;
        m_state.PC = pop16();
    }
}

void CPU8080::RLC()
{
    // rotate left
    u8 newCarry = m_state.A >> 7;
    m_state.A <<= 1;
    m_state.A |= newCarry;

    m_state.F.Carry = newCarry;
}

void CPU8080::RRC()
{
    // rotate right
    u8 newCarry = m_state.A & 1;
    m_state.A >>= 1;
    m_state.A |= newCarry << 7;

    m_state.F.Carry = newCarry;
}

void CPU8080::RST(u8 vector)
{
    push16(m_state.PC);
    m_state.PC = vector * 8;
}

void CPU8080::SUB(u8 value)
{
    u16 result = (s16)m_state.A - (s16)value;
    u8 result4bit = (s8)(m_state.A & 0xF) - (s8)(value & 0xF);
    m_state.A = result & 0xFF;

    m_state.F.Zero = (m_state.A == 0);
    m_state.F.HalfCarry = (result4bit >> 4);
    m_state.F.Carry = (result >> 8);
    m_state.F.Sign = (result >> 7);
    m_state.F.Parity = ((std::bitset<8>(m_state.A).count() % 2) == 0);
}

void CPU8080::SBB(u8 value)
{
    u16 result = (s16)m_state.A - (s16)value - m_state.F.Carry;
    u8 result4bit = (s8)(m_state.A & 0xF) - (s8)(value & 0xF) - m_state.F.Carry;
    m_state.A = result & 0xFF;

    m_state.F.Zero = (m_state.A == 0);
    m_state.F.HalfCarry = (result4bit >> 4);
    m_state.F.Carry = (result >> 8);
    m_state.F.Sign = (result >> 7);
    m_state.F.Parity = ((std::bitset<8>(m_state.A).count() % 2) == 0);
}

void CPU8080::XCHG()
{
    u16 temp = m_state.HL;
    m_state.HL = m_state.DE;
    m_state.DE = temp;
}

void CPU8080::XOR(u8 value)
{
    m_state.A ^= value;

    m_state.F.Zero = (m_state.A == 0);
    m_state.F.Parity = ((std::bitset<8>(m_state.A).count() % 2) == 0);
}

void CPU8080::XTHL()
{
    u8 temp = m_state.L;
    m_state.L = load8(m_state.SP);
    store8(m_state.SP, temp);

    temp = m_state.H;
    m_state.H = load8(m_state.SP + 1);
    store8(m_state.SP + 1, temp);
}
