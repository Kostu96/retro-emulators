#include "cpu8080.hpp"
#include "cpu_operations.hpp"

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
            u8 opcode = load8(PC++);

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
    case 0x01: BC = load16(PC); PC += 2;         break;
    case 0x02: store8(BC, A);                    break;
    case 0x03: BC++;                             break;
    case 0x04: INCR(B);                          break;
    case 0x05: DECR(B);                          break;
    case 0x06: B = load8(PC++);                  break;
    case 0x07: RLC();                            break;
                                                 
    case 0x09: ADDHL(BC);                        break;
    case 0x0A: A = load8(BC);                    break;
    case 0x0B: BC--;                             break;
    case 0x0C: INCR(C);                          break;
    case 0x0D: DECR(C);                          break;
    case 0x0E: C = load8(PC++);                  break;
    case 0x0F: RRC();                            break;
                                                 
    case 0x11: DE = load16(PC); PC += 2;         break;
    case 0x12: store8(DE, A);                    break;
    case 0x13: DE++;                             break;
    case 0x14: INCR(D);                          break;
    case 0x15: DECR(D);                          break;
    case 0x16: D = load8(PC++);                  break;
    case 0x17: RAL();                            break;
                                                 
    case 0x19: ADDHL(DE);                        break;
    case 0x1A: A = load8(DE);                    break;
    case 0x1B: DE--;                             break;
    case 0x1C: INCR(E);                          break;
    case 0x1D: DECR(E);                          break;
    case 0x1E: E = load8(PC++);                  break;
    case 0x1F: RAR();                            break;
    
    case 0x21: HL = load16(PC); PC += 2;         break;
    case 0x22: store16(load16(PC), HL); PC += 2; break;
    case 0x23: HL++;                             break;
    case 0x24: INCR(H);                          break;
    case 0x25: DECR(H);                          break;
    case 0x26: H = load8(PC++);                  break;
    case 0x27: DAA();                            break;

    case 0x29: ADDHL(HL);                        break;
    case 0x2A: HL = load16(load16(PC)); PC += 2; break;
    case 0x2B: HL--;                             break;
    case 0x2C: INCR(L);                          break;
    case 0x2D: DECR(L);                          break;
    case 0x2E: L = load8(PC++);                  break;
    case 0x2F: A = ~A;                           break;

    case 0x31: SP = load16(PC); PC += 2;         break;
    case 0x32: store8(load16(PC), A); PC += 2;   break;
    case 0x33: SP++;                             break;
    case 0x34: INCM();                           break;
    case 0x35: DECM();                           break;
    case 0x36: store8(HL, load8(PC++));          break;
    case 0x37: F.Carry = 1;                      break;

    case 0x39: ADDHL(SP);                        break;
    case 0x3A: A = load8(load16(PC)); PC += 2;   break;
    case 0x3B: SP--;                             break;
    case 0x3C: INCR(A);                          break;
    case 0x3D: DECR(A);                          break;
    case 0x3E: A = load8(PC++);                  break;
    case 0x3F: F.Carry = ~F.Carry;               break;
    case 0x40: B = B;                            break;
    case 0x41: B = C;                            break;
    case 0x42: B = D;                            break;
    case 0x43: B = E;                            break;
    case 0x44: B = H;                            break;
    case 0x45: B = L;                            break;
    case 0x46: B = load8(HL);                    break;
    case 0x47: B = A;                            break;
    case 0x48: C = B;                            break;
    case 0x49: C = C;                            break;
    case 0x4A: C = D;                            break;
    case 0x4B: C = E;                            break;
    case 0x4C: C = H;                            break;
    case 0x4D: C = L;                            break;
    case 0x4E: C = load8(HL);                    break;
    case 0x4F: C = A;                            break;
    case 0x50: D = B;                            break;
    case 0x51: D = C;                            break;
    case 0x52: D = D;                            break;
    case 0x53: D = E;                            break;
    case 0x54: D = H;                            break;
    case 0x55: D = L;                            break;
    case 0x56: D = load8(HL);                    break;
    case 0x57: D = A;                            break;
    case 0x58: E = B;                            break;
    case 0x59: E = C;                            break;
    case 0x5A: E = D;                            break;
    case 0x5B: E = E;                            break;
    case 0x5C: E = H;                            break;
    case 0x5D: E = L;                            break;
    case 0x5E: E = load8(HL);                    break;
    case 0x5F: E = A;                            break;
    case 0x60: H = B;                            break;
    case 0x61: H = C;                            break;
    case 0x62: H = D;                            break;
    case 0x63: H = E;                            break;
    case 0x64: H = H;                            break;
    case 0x65: H = L;                            break;
    case 0x66: H = load8(HL);                    break;
    case 0x67: H = A;                            break;
    case 0x68: L = B;                            break;
    case 0x69: L = C;                            break;
    case 0x6A: L = D;                            break;
    case 0x6B: L = E;                            break;
    case 0x6C: L = H;                            break;
    case 0x6D: L = L;                            break;
    case 0x6E: L = load8(HL);                    break;
    case 0x6F: L = A;                            break;
    case 0x70: store8(HL, B);                    break;
    case 0x71: store8(HL, C);                    break;
    case 0x72: store8(HL, D);                    break;
    case 0x73: store8(HL, E);                    break;
    case 0x74: store8(HL, H);                    break;
    case 0x75: store8(HL, L);                    break;
    case 0x76: m_isHalted = true;                break;
    case 0x77: store8(HL, A);                    break;
    case 0x78: A = B;                            break;
    case 0x79: A = C;                            break;
    case 0x7A: A = D;                            break;
    case 0x7B: A = E;                            break;
    case 0x7C: A = H;                            break;
    case 0x7D: A = L;                            break;
    case 0x7E: A = load8(HL);                    break;
    case 0x7F: A = A;                            break;
    case 0x80: ADD(B);                           break;
    case 0x81: ADD(C);                           break;
    case 0x82: ADD(D);                           break;
    case 0x83: ADD(E);                           break;
    case 0x84: ADD(H);                           break;
    case 0x85: ADD(L);                           break;
    case 0x86: ADD(load8(HL));                   break;
    case 0x87: ADD(A);                           break;
    case 0x88: ADC(B);                           break;
    case 0x89: ADC(C);                           break;
    case 0x8A: ADC(D);                           break;
    case 0x8B: ADC(E);                           break;
    case 0x8C: ADC(H);                           break;
    case 0x8D: ADC(L);                           break;
    case 0x8E: ADC(load8(HL));                   break;
    case 0x8F: ADC(A);                           break;
    case 0x90: SUB(B);                           break;
    case 0x91: SUB(C);                           break;
    case 0x92: SUB(D);                           break;
    case 0x93: SUB(E);                           break;
    case 0x94: SUB(H);                           break;
    case 0x95: SUB(L);                           break;
    case 0x96: SUB(load8(HL));                   break;
    case 0x97: SUB(A);                           break;
    case 0x98: SBB(B);                           break;
    case 0x99: SBB(C);                           break;
    case 0x9A: SBB(D);                           break;
    case 0x9B: SBB(E);                           break;
    case 0x9C: SBB(H);                           break;
    case 0x9D: SBB(L);                           break;
    case 0x9E: SBB(load8(HL));                   break;
    case 0x9F: SBB(A);                           break;
    case 0xA0: AND(B);                           break;
    case 0xA1: AND(C);                           break;
    case 0xA2: AND(D);                           break;
    case 0xA3: AND(E);                           break;
    case 0xA4: AND(H);                           break;
    case 0xA5: AND(L);                           break;
    case 0xA6: AND(load8(HL));                   break;
    case 0xA7: AND(A);                           break;
    case 0xA8: XOR(B);                           break;
    case 0xA9: XOR(C);                           break;
    case 0xAA: XOR(D);                           break;
    case 0xAB: XOR(E);                           break;
    case 0xAC: XOR(H);                           break;
    case 0xAD: XOR(L);                           break;
    case 0xAE: XOR(load8(HL));                   break;
    case 0xAF: XOR(A);                           break;
    case 0xB0: OR(B);                            break;
    case 0xB1: OR(C);                            break;
    case 0xB2: OR(D);                            break;
    case 0xB3: OR(E);                            break;
    case 0xB4: OR(H);                            break;
    case 0xB5: OR(L);                            break;
    case 0xB6: OR(load8(HL));                    break;
    case 0xB7: OR(A);                            break;
    case 0xB8: CMP(B);                           break;
    case 0xB9: CMP(C);                           break;
    case 0xBA: CMP(D);                           break;
    case 0xBB: CMP(E);                           break;
    case 0xBC: CMP(H);                           break;
    case 0xBD: CMP(L);                           break;
    case 0xBE: CMP(load8(HL));                   break;
    case 0xBF: CMP(A);                           break;
    case 0xC0: RET(!F.Carry);                    break;
    case 0xC1: BC = pop16();                     break;
    case 0xC2: JMP(!F.Zero);                     break;
    case 0xC3: JMP(true);                        break;
    case 0xC4: CALL(!F.Zero);                    break;
    case 0xC5: push16(BC);                       break;
    case 0xC6: ADD(load8(PC++));                 break;
    case 0xC7: RST(0);                           break;
    case 0xC8: RET(F.Zero);                      break;
    case 0xC9: RET(true);                        break;
    case 0xCA: JMP(F.Zero);                      break;

    case 0xCC: CALL(F.Zero);                     break;
    case 0xCD: CALL(true);                       break;
    case 0xCE: ADC(load8(PC++));                 break;
    case 0xCF: RST(1);                           break;
    case 0xD0: RET(!F.Carry);                    break;
    case 0xD1: DE = pop16();                     break;
    case 0xD2: JMP(!F.Carry);                    break;
    case 0xD3: load8(PC++);                      break; /* to be implemented */
    case 0xD4: CALL(!F.Carry);                   break;
    case 0xD5: push16(DE);                       break;
    case 0xD6: SUB(load8(PC++));                 break;
    case 0xD7: RST(2);                           break;
    case 0xD8: RET(F.Carry);                     break;

    case 0xDA: JMP(F.Carry);                     break;
    case 0xDB: load8(PC++);                      break; /* to be implemented */
    case 0xDC: CALL(F.Carry);                    break;

    case 0xDE: SBB(load8(PC++));                 break;
    case 0xDF: RST(3);                           break;
    case 0xE0: RET(!F.Parity);                   break;
    case 0xE1: HL = pop16();                     break;
    case 0xE2: JMP(!F.Parity);                   break;
    case 0xE3: XTHL();                           break;
    case 0xE4: CALL(!F.Parity);                  break;
    case 0xE5: push16(HL);                       break;
    case 0xE6: AND(load8(PC++));                 break;
    case 0xE7: RST(4);                           break;
    case 0xE8: RET(F.Parity);                    break;
    case 0xE9: PC = HL;                          break;
    case 0xEA: JMP(F.Parity);                    break;
    case 0xEB: XCHG();                           break;
    case 0xEC: CALL(F.Parity);                   break;

    case 0xEE: XOR(load8(PC++));                 break;
    case 0xEF: RST(5);                           break;
    case 0xF0: RET(!F.Sign);                     break;
    case 0xF1: AF = pop16(); F.byte &= 0xF0;     break;
    case 0xF2: JMP(!F.Sign);                     break;
    case 0xF3: m_interruptEnabled = false;       break;
    case 0xF4: CALL(!F.Sign);                    break;
    case 0xF5: push16(AF);                       break;
    case 0xF6: OR(load8(PC++));                  break;
    case 0xF7: RST(6);                           break;
    case 0xF8: RET(F.Sign);                      break;
    case 0xF9: SP = HL;                          break;
    case 0xFA: JMP(F.Sign);                      break;
    case 0xFB: m_EIRequested = true;             break;
    case 0xFC: CALL(F.Sign);                     break;

    case 0xFE: CMP(load8(PC++));                 break;
    case 0xFF: RST(7);                           break;
    default:
        assert(false && "Unhandled opcode!");
    }
}

void CPU8080::ADD(u8 value)
{
    u16 result;
    u8 result4bit;
    add8(A, value, 0, result, result4bit);
    A = result;

    F.Zero = (A == 0);
    F.HalfCarry = (result4bit >> 4);
    F.Carry = (result >> 8);
    F.Sign = (result >> 7);
    F.Parity = ((std::bitset<8>(A).count() % 2) == 0);
}

void CPU8080::ADC(u8 value)
{
    u16 result;
    u8 result4bit;
    add8(A, value, F.Carry, result, result4bit);
    A = result;

    F.Zero = (A == 0);
    F.HalfCarry = (result4bit >> 4);
    F.Carry = (result >> 8);
    F.Sign = (result >> 7);
    F.Parity = ((std::bitset<8>(A).count() % 2) == 0);
}

void CPU8080::ADDHL(u16 value)
{
    u32 result;
    u16 result12bit;
    add16(HL, value, result, result12bit);
    HL = result;

    F.Carry = (result >> 16);
}

void CPU8080::AND(u8 value)
{
    A &= value;

    F.Zero = (A == 0);
    F.Parity = ((std::bitset<8>(A).count() % 2) == 0);
}

void CPU8080::CALL(bool flag)
{
    u16 address = load16(PC);
    PC += 2;
    if (flag) {
        m_conditionalTaken = true;
        push16(PC);
        PC = address;
    }
}

void CPU8080::CMP(u8 value)
{
    u16 result;
    u8 result4bit;
    sub8(A, value, 0, result, result4bit);

    F.Zero = (result == 0);
    F.HalfCarry = (result4bit >> 4);
    F.Carry = (result >> 8);
    F.Sign = (result >> 7);
    F.Parity = ((std::bitset<8>(A).count() % 2) == 0);
}

void CPU8080::DAA()
{
    u16 value = A;

    if ((value & 0x0F) > 9 || F.HalfCarry)
        value += 0x06;

    if ((value >> 4) > 9 || F.Carry)
        value += 0x60;

    A = value;

    F.Carry = (value >> 8);
    F.Parity = ((std::bitset<8>(A).count() % 2) == 0);
    F.Sign = (A >> 7);
    F.Zero = (A == 0);
}

void CPU8080::DECM()
{
    u8 value = load8(HL);
    DECR(value);
    store8(HL, value);
}

void CPU8080::DECR(u8& reg)
{
    u8 halfResult = (s8)(reg & 0xF) - 1;
    reg--;

    F.HalfCarry = (halfResult >> 4);
    F.Zero = (reg == 0);
    F.Sign = (reg >> 7);
    F.Parity = ((std::bitset<8>(reg).count() % 2) == 0);
}

void CPU8080::INCM()
{
    u8 value = load8(HL);
    INCR(value);
    store8(HL, value);
}

void CPU8080::INCR(u8& reg)
{
    u8 halfResult = (reg & 0xF) + 1;
    reg++;

    F.HalfCarry = (halfResult >> 4);
    F.Zero = (reg == 0);
    F.Sign = (reg >> 7);
    F.Parity = ((std::bitset<8>(reg).count() % 2) == 0);
}

void CPU8080::JMP(bool flag)
{
    u16 address = load16(PC);
    PC += 2;
    if (flag) {
        m_conditionalTaken = true;
        PC = address;
    }
}

void CPU8080::OR(u8 value)
{
    A |= value;

    F.Zero = (A == 0);
    F.Parity = ((std::bitset<8>(A).count() % 2) == 0);
}

void CPU8080::RAL()
{
    u8 newCarry;
    rotateLeftWithCarry(A, F.Carry, newCarry);

    F.Carry = newCarry;
}

void CPU8080::RAR()
{
    u8 newCarry;
    rotateRightWithCarry(A, F.Carry, newCarry);

    F.Carry = newCarry;
}

void CPU8080::RET(bool flag)
{
    if (flag) {
        m_conditionalTaken = true;
        PC = pop16();
    }
}

void CPU8080::RLC()
{
    u8 newCarry;
    rotateLeft(A, newCarry);

    F.Carry = newCarry;
}

void CPU8080::RRC()
{
    u8 newCarry;
    rotateRight(A, newCarry);

    F.Carry = newCarry;
}

void CPU8080::RST(u8 vector)
{
    push16(PC);
    PC = vector * 8;
}

void CPU8080::SUB(u8 value)
{
    u16 result;
    u8 result4bit;
    sub8(A, value, 0, result, result4bit);
    A = result;

    F.Zero = (A == 0);
    F.HalfCarry = (result4bit >> 4);
    F.Carry = (result >> 8);
    F.Sign = (result >> 7);
    F.Parity = ((std::bitset<8>(A).count() % 2) == 0);
}

void CPU8080::SBB(u8 value)
{
    u16 result;
    u8 result4bit;
    sub8(A, value, F.Carry, result, result4bit);
    A = result;

    F.Zero = (A == 0);
    F.HalfCarry = (result4bit >> 4);
    F.Carry = (result >> 8);
    F.Sign = (result >> 7);
    F.Parity = ((std::bitset<8>(A).count() % 2) == 0);
}

void CPU8080::XCHG()
{
    u16 temp = HL;
    HL = DE;
    DE = temp;
}

void CPU8080::XOR(u8 value)
{
    A ^= value;

    F.Zero = (A == 0);
    F.Parity = ((std::bitset<8>(A).count() % 2) == 0);
}

void CPU8080::XTHL()
{
    u8 temp = L;
    L = load8(SP);
    store8(SP, temp);

    temp = H;
    H = load8(SP + 1);
    store8(SP + 1, temp);
}
