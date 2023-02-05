#include "cpu8080.hpp"

#include <bitset>
#include <cassert>


u8 CPU8080::getCarryFlag()
{
    switch (m_mode)
    {
    case Mode::Intel8080: return m_state.F.i8080.Carry;
    case Mode::GameBoy: return m_state.F.gb.Carry;
    }

    assert(false && "Unhandled mode");
    return 0xFF;
}

void CPU8080::setCarryFlag(u8 value)
{
    switch (m_mode)
    {
    case Mode::Intel8080: m_state.F.i8080.Carry = value; break;
    case Mode::GameBoy: m_state.F.gb.Carry = value; break;
    default:
        assert(false && "Unhandled mode");
    }
}

u8 CPU8080::getHalfCarryFlag()
{
    switch (m_mode)
    {
    case Mode::Intel8080: return m_state.F.i8080.HalfCarry;
    case Mode::GameBoy: return m_state.F.gb.HalfCarry;
    }

    assert(false && "Unhandled mode");
    return 0xFF;
}

void CPU8080::setHalfCarryFlag(u8 value)
{
    switch (m_mode)
    {
    case Mode::Intel8080: m_state.F.i8080.HalfCarry = value; break;
    case Mode::GameBoy: m_state.F.gb.HalfCarry = value; break;
    default:
        assert(false && "Unhandled mode");
    }
}

u8 CPU8080::getSubtractFlag()
{
    switch (m_mode)
    {
    case Mode::GameBoy: return m_state.F.gb.Subtract;
    }

    assert(false && "Unhandled mode");
    return 0xFF;
}

void CPU8080::setSubtractFlag(u8 value)
{
    switch (m_mode)
    {
    case Mode::GameBoy: m_state.F.gb.Subtract = value; break;
    default:
        assert(false && "Unhandled mode");
    }
}

u8 CPU8080::getZeroFlag()
{
    switch (m_mode)
    {
    case Mode::Intel8080: return m_state.F.i8080.Zero;
    case Mode::GameBoy: return m_state.F.gb.Zero;
    }

    assert(false && "Unhandled mode");
    return 0xFF;
}

void CPU8080::setZeroFlag(u8 value)
{
    switch (m_mode)
    {
    case Mode::Intel8080: m_state.F.i8080.Zero = value; break;
    case Mode::GameBoy: m_state.F.gb.Zero = value; break;
    default:
        assert(false && "Unhandled mode");
    }
}

u8 CPU8080::getSignFlag()
{
    switch (m_mode)
    {
    case Mode::Intel8080: return m_state.F.i8080.Sign;
    }

    assert(false && "Unhandled mode");
    return 0xFF;
}

void CPU8080::setSignFlag(u8 value)
{
    switch (m_mode)
    {
    case Mode::Intel8080: m_state.F.i8080.Sign = value; break;
    default:
        assert(false && "Unhandled mode");
    }
}

u8 CPU8080::getParityFlag()
{
    switch (m_mode)
    {
    case Mode::Intel8080: return m_state.F.i8080.Parity;
    }

    assert(false && "Unhandled mode");
    return 0xFF;
}

void CPU8080::setParityFlag(u8 value)
{
    switch (m_mode)
    {
    case Mode::Intel8080: m_state.F.i8080.Parity = value; break;
    default:
        assert(false && "Unhandled mode");
    }
}

void CPU8080::reset()
{
    m_state.AF = 0;
    m_state.BC = 0;
    m_state.DE = 0;
    m_state.HL = 0;
    m_state.SP = 0xF000;
    m_state.PC = 0;
    m_state.InterruptEnabled = false;

    m_prefixMode = false;
}

void CPU8080::clock()
{
    u8 opcode = load8(m_state.PC++);

    if (m_prefixMode)
    {
        m_prefixMode = false;
        prefixInstruction(opcode);
    }
    else 
        standardInstruction(opcode);
}

void CPU8080::interrupt(u8 vector)
{
    if (m_state.InterruptEnabled)
    {
        m_state.InterruptEnabled = false;
        RST(vector);
    }
}

void CPU8080::standardInstruction(u8 opcode)
{
    switch (opcode)
    {
    case 0x00: break;
    case 0x01: m_state.BC = load16(m_state.PC); m_state.PC += 2; break;
    case 0x02: store8(m_state.BC, m_state.A); break;
    case 0x03: INCRP(m_state.BC); break;
    case 0x04: INCR(m_state.B); break;
    case 0x05: DECR(m_state.B); break;
    case 0x06: m_state.B = load8(m_state.PC++); break;
    case 0x07: RLC(); break;
    case 0x08: {
        switch (m_mode)
        {
        case Mode::GameBoy:
            LDMM(load16(m_state.PC), m_state.SP);
            m_state.PC += 2;
            break;
        default:
        case Mode::Intel8080:
            assert(false && "Unhandled standard instruction");
        }
    } break;
    case 0x09: ADDHL(m_state.BC); break;
    case 0x0A: m_state.A = load8(m_state.BC); break;
    case 0x0B: DECRP(m_state.BC); break;
    case 0x0C: INCR(m_state.C); break;
    case 0x0D: DECR(m_state.C); break;
    case 0x0E: m_state.C = load8(m_state.PC++); break;
    case 0x0F: RRC(); break;

    case 0x11: m_state.DE = load16(m_state.PC); m_state.PC += 2; break;
    case 0x12: store8(m_state.DE, m_state.A); break;
    case 0x13: INCRP(m_state.DE); break;
    case 0x14: INCR(m_state.D); break;
    case 0x15: DECR(m_state.D); break;
    case 0x16: m_state.D = load8(m_state.PC++); break;
    case 0x17: RAL(); break;
    case 0x18: {
        switch (m_mode)
        {
        case Mode::GameBoy:
            JR(true);
            break;
        default:
        case Mode::Intel8080:
            assert(false && "Unhandled standard instruction");
        }
    } break;

    case 0x20: {
        switch (m_mode)
        {
        case Mode::GameBoy:
            JR(!getZeroFlag());
            break;
        default:
        case Mode::Intel8080:
            assert(false && "Unhandled standard instruction");
        }
    } break;
    case 0x19: ADDHL(m_state.DE); break;
    case 0x1A: m_state.A = load8(m_state.DE); break;
    case 0x1B: DECRP(m_state.DE); break;
    case 0x1C: INCR(m_state.E); break;
    case 0x1D: DECR(m_state.E); break;
    case 0x1E: m_state.E = load8(m_state.PC++); break;
    case 0x1F: RAR(); break;

    case 0x21: m_state.HL = load16(m_state.PC); m_state.PC += 2; break;
    case 0x22: {
        switch (m_mode)
        {
        case Mode::GameBoy:
            LDM(m_state.HL++, m_state.A);
            break;
        default:
        case Mode::Intel8080:
            store16(load16(m_state.PC), m_state.HL);
            m_state.PC += 2;
            break;
        }
    } break; 
    case 0x23: INCRP(m_state.HL); break;
    case 0x24: INCR(m_state.H); break;
    case 0x25: DECR(m_state.H); break;
    case 0x26: m_state.H = load8(m_state.PC++); break;
    case 0x27: DAA(); break;
    case 0x28: {
        switch (m_mode)
        {
        case Mode::GameBoy:
            JR(getZeroFlag());
            break;
        default:
        case Mode::Intel8080:
            assert(false && "Unhandled standard instruction");
        }
    } break;
    case 0x29: ADDHL(m_state.HL); break;
    case 0x2A: {
        switch (m_mode)
        {
        case Mode::GameBoy:
            m_state.A = load8(m_state.HL++);
            break;
        default:
        case Mode::Intel8080:
            m_state.HL = load16(load16(m_state.PC));
            m_state.PC += 2;
            break;
        }
    } break;
    case 0x2B: DECRP(m_state.HL); break;
    case 0x2C: INCR(m_state.L); break;
    case 0x2D: DECR(m_state.L); break;
    case 0x2E: m_state.L = load8(m_state.PC++); break;
    case 0x2F: CMA(); break;
    case 0x30: {
        switch (m_mode)
        {
        case Mode::GameBoy:
            JR(!getCarryFlag());
            break;
        default:
        case Mode::Intel8080:
            assert(false && "Unhandled standard instruction");
        }
    } break;
    case 0x31: m_state.SP = load16(m_state.PC); m_state.PC += 2; break;
    case 0x32: {
        switch (m_mode)
        {
        case Mode::GameBoy:
            LDM(m_state.HL--, m_state.A);
            break;
        default:
        case Mode::Intel8080:
            LDM(load16(m_state.PC), m_state.A);
            m_state.PC += 2;
            break;
        }
    } break;
    case 0x33: INCRP(m_state.SP); break;
    case 0x34: INCM(); break;
    case 0x35: DECM(); break;
    case 0x36: LDM(m_state.HL, load8(m_state.PC++)); break;
    case 0x37: STC(); break;

    case 0x39: ADDHL(m_state.SP); break;
    case 0x3A: {
        switch (m_mode)
        {
        case Mode::GameBoy:
            m_state.A = load8(m_state.HL--);
            break;
        default:
        case Mode::Intel8080:
            m_state.A = load8(load16(m_state.PC));
            m_state.PC += 2;
            break;
        }
    } break;
    case 0x3B: DECRP(m_state.SP); break;
    case 0x3C: INCR(m_state.A); break;
    case 0x3D: DECR(m_state.A); break;
    case 0x3E: m_state.A = load8(m_state.PC++); break;
    case 0x3F: CMC(); break;
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
    case 0x70: LDM(m_state.HL, m_state.B); break;
    case 0x71: LDM(m_state.HL, m_state.C); break;
    case 0x72: LDM(m_state.HL, m_state.D); break;
    case 0x73: LDM(m_state.HL, m_state.E); break;
    case 0x74: LDM(m_state.HL, m_state.H); break;
    case 0x75: LDM(m_state.HL, m_state.L); break;

    case 0x77: LDM(m_state.HL, m_state.A); break;
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
    case 0xC0: RET(!getZeroFlag()); break;
    case 0xC1: m_state.BC = pop16(); break;
    case 0xC2: JMP(!getZeroFlag()); break;
    case 0xC3: JMP(true); break;
    case 0xC4: CALL(!getZeroFlag()); break;
    case 0xC5: push16(m_state.BC); break;
    case 0xC6: ADD(load8(m_state.PC++)); break;
    case 0xC7: RST(0); break;
    case 0xC8: RET(getZeroFlag()); break;
    case 0xC9: RET(true); break;
    case 0xCA: JMP(getZeroFlag()); break;
    case 0xCB: {
        switch (m_mode)
        {
        case Mode::GameBoy:
            m_prefixMode = true;
            break;
        default:
        case Mode::Intel8080:
            assert(false && "Unhandled standard instruction");
            break;
        }
    } break;
    case 0xCC: CALL(getZeroFlag()); break;
    case 0xCD: CALL(true); break;
    case 0xCE: ADC(load8(m_state.PC++)); break;
    case 0xCF: RST(1); break;
    case 0xD0: RET(!getCarryFlag()); break;
    case 0xD1: m_state.DE = pop16(); break;
    case 0xD2: JMP(!getCarryFlag()); break;
    case 0xD3: load8(m_state.PC++); /* to be implemented */ break;
    case 0xD4: CALL(!getCarryFlag()); break;
    case 0xD5: push16(m_state.DE); break;
    case 0xD6: SUB(load8(m_state.PC++)); break;
    case 0xD7: RST(2); break;
    case 0xD8: RET(getCarryFlag()); break;

    case 0xDA: JMP(getCarryFlag()); break;
    case 0xDB: load8(m_state.PC++); /* to be implemented */ break;
    case 0xDC: CALL(getCarryFlag()); break;

    case 0xDE: SBB(load8(m_state.PC++)); break;
    case 0xDF: RST(3); break;
    case 0xE0: {
        switch (m_mode)
        {
        case Mode::GameBoy:
            LDM(0xFF00 | load8(m_state.PC++), m_state.A);
            break;
        default:
        case Mode::Intel8080:
            RET(!getParityFlag());
            break;
        }
    } break;
    case 0xE1: m_state.HL = pop16(); break;
    case 0xE2: {
        switch (m_mode)
        {
        case Mode::GameBoy:
            LDM(0xFF00 | m_state.C, m_state.A);
            break;
        default:
        case Mode::Intel8080:
            JMP(!getParityFlag());
            break;
        }
    } break;
    case 0xE3: XTHL(); break;
    case 0xE4: CALL(!getParityFlag()); break;
    case 0xE5: push16(m_state.HL); break;
    case 0xE6: AND(load8(m_state.PC++)); break;
    case 0xE7: RST(4); break;
    case 0xE8: RET(getParityFlag()); break;
    case 0xE9: m_state.PC = m_state.HL; break;
    case 0xEA: {
        switch (m_mode)
        {
        case Mode::GameBoy:
            LDM(load16(m_state.PC), m_state.A);
            m_state.PC += 2;
            break;
        default:
        case Mode::Intel8080:
            JMP(getParityFlag());
            break;
        }
    } break;
    case 0xEB: XCHG(); break;
    case 0xEC: CALL(getParityFlag()); break;

    case 0xEE: XOR(load8(m_state.PC++)); break;
    case 0xEF: RST(5); break;
    case 0xF0: {
        switch (m_mode)
        {
        case Mode::GameBoy:
            m_state.A = load8(0xFF00 | load8(m_state.PC++));
            break;
        default:
        case Mode::Intel8080:
            RET(!getSignFlag());
            break;
        }
    } break;
    case 0xF1: m_state.AF = pop16(); break;
    case 0xF2: {
        switch (m_mode)
        {
        case Mode::GameBoy:
            m_state.A = load8(0xFF00 | m_state.C);
            break;
        default:
        case Mode::Intel8080:
            JMP(!getSignFlag());
            break;
        }
    } break;
    case 0xF3: m_state.InterruptEnabled = false; break;
    case 0xF4: CALL(!getSignFlag()); break;
    case 0xF5: push16(m_state.AF); break;
    case 0xF6: OR(load8(m_state.PC++)); break;
    case 0xF7: RST(6); break;
    case 0xF8: {
        switch (m_mode)
        {
        case Mode::GameBoy: {
            u8 imm = load8(m_state.PC++);
            u32 result = m_state.SP + (s8)imm;
            u8 halfResult = (m_state.SP & 0xF) + (imm & 0xF);
            m_state.HL = result;
            setZeroFlag(0);
            setSubtractFlag(0);
            setCarryFlag(result >> 16);
            setHalfCarryFlag(halfResult >> 4);
        } break;
        default:
        case Mode::Intel8080:
            RET(getSignFlag());
            break;
        }
    } break;
    case 0xF9: m_state.SP = m_state.HL; break;
    case 0xFA: {
        switch (m_mode)
        {
        case Mode::GameBoy:
            m_state.A = load8(load16(m_state.PC));
            m_state.PC += 2;
            break;
        default:
        case Mode::Intel8080:
            JMP(getSignFlag());
            break;
        }
    } break;
    case 0xFB: m_state.InterruptEnabled = true; break;
    case 0xFC: CALL(getSignFlag()); break;

    case 0xFE: CMP(load8(m_state.PC++)); break;
    case 0xFF: RST(7); break;
    default:
        assert(false && "Unhandled standard instruction");
    }
}

void CPU8080::prefixInstruction(u8 opcode)
{
    switch (opcode)
    {
    case 0x11: RL(m_state.C); break;

    case 0x19: RR(m_state.C); break;
    case 0x1A: RR(m_state.D); break;

    case 0x37: SWAP(m_state.A); break;
    case 0x38: SRL(m_state.B); break;

    case 0x3F: SRL(m_state.A); break;

    case 0x7C: BIT(m_state.H, 7); break;

    case 0x7E: BIT(load8(m_state.HL), 7); break;

    case 0xBE: RESHL(7); break;

    case 0xCE: SETHL(1); break;

    case 0xE6: SETHL(4); break;

    case 0xF6: SETHL(6); break;

    case 0xFE: SETHL(7); break;
    default:
        assert(false && "Unhandled prefix instruction");
    }
}

void CPU8080::SWAP(u8& reg)
{
    u8 l = reg & 0x0F;
    u8 h = reg & 0xF0;
    reg = (l << 4) | h;
    setZeroFlag(reg == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);
    setCarryFlag(0);
}

void CPU8080::SRL(u8& reg)
{
    setCarryFlag(reg & 1);
    reg >>= 1;
    setZeroFlag(reg == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);
}

void CPU8080::RR(u8& reg)
{
    u8 temp = getCarryFlag();
    setCarryFlag(reg & 1);
    reg >>= 1;
    reg |= temp << 7;
    setZeroFlag(reg == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);
}

void CPU8080::BIT(u8 value, u8 bit)
{
    setZeroFlag(!((value >> bit) & 1));
    setSubtractFlag(0);
    setHalfCarryFlag(1);
}

void CPU8080::SETHL(u8 bit)
{
    u8 value = load8(m_state.HL);
    u8 mask = 1 << bit;
    value |= mask;
    store8(m_state.HL, value);
}

void CPU8080::RES(u8& reg, u8 bit)
{

}

void CPU8080::RESHL(u8 bit)
{
    u8 value = load8(m_state.HL);
    u8 mask = ~(1 << bit);
    value &= mask;
    store8(m_state.HL, value);
}

void CPU8080::LDM(u16 address, u8 value)
{
    store8(address, value);
}

void CPU8080::LDMM(u16 address, u16 value)
{
    store16(address, value);
}

void CPU8080::XCHG()
{
    u16 temp = m_state.HL;
    m_state.HL = m_state.DE;
    m_state.DE = temp;
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

void CPU8080::ADD(u8 value)
{
    u16 result = m_state.A + value;
    u8 result4bit = (m_state.A & 0xF) + (value & 0xF);
    m_state.A = result;
    setZeroFlag(m_state.A == 0);
    setHalfCarryFlag(result4bit >> 4);
    setCarryFlag(result >> 8);

    switch (m_mode)
    {
    case Mode::GameBoy:
        setSubtractFlag(0);
        break;
    case Mode::Intel8080:
        setSignFlag(result >> 7);
        setParityFlag((std::bitset<8>(m_state.A).count() % 2) == 0);
        break;
    }
}

void CPU8080::ADC(u8 value)
{
    u16 result = m_state.A + value + getCarryFlag();
    u8 result4bit = (m_state.A & 0xF) + (value & 0xF);
    m_state.A = result;
    setZeroFlag(m_state.A == 0);
    setHalfCarryFlag(result4bit >> 4);
    setCarryFlag(result >> 8);

    switch (m_mode)
    {
    case Mode::GameBoy:
        setSubtractFlag(0);
        break;
    case Mode::Intel8080:
        setSignFlag(result >> 7);
        setParityFlag((std::bitset<8>(m_state.A).count() % 2) == 0);
        break;
    }
}

void CPU8080::SUB(u8 value)
{
    u16 result = (s16)m_state.A - (s16)value;
    u8 result4bit = (s8)m_state.A - (s8)value;
    m_state.A = result;
    setZeroFlag(m_state.A == 0);
    setHalfCarryFlag(result4bit >> 4);
    setCarryFlag(result >> 8);

    switch (m_mode)
    {
    case Mode::GameBoy:
        setSubtractFlag(1);
        break;
    case Mode::Intel8080:
        setSignFlag(result >> 7);
        setParityFlag((std::bitset<8>(m_state.A).count() % 2) == 0);
        break;
    }
}

void CPU8080::SBB(u8 value)
{
    u16 result = (s16)m_state.A - (s16)value - getCarryFlag();
    u8 result4bit = (s8)m_state.A - (s8)value;
    m_state.A = result;
    setZeroFlag(m_state.A == 0);
    setHalfCarryFlag(result4bit >> 4);
    setCarryFlag(result >> 8);

    switch (m_mode)
    {
    case Mode::GameBoy:
        setSubtractFlag(1);
        break;
    case Mode::Intel8080:
        setSignFlag(result >> 7);
        setParityFlag((std::bitset<8>(m_state.A).count() % 2) == 0);
        break;
    }
}

void CPU8080::ADDHL(u16 value)
{
    u32 result = m_state.HL + value;
    m_state.HL = result;
    setCarryFlag(result >> 16);
}

void CPU8080::CMP(u8 value)
{
    u16 result = (s16)m_state.A - (s16)value;
    u8 result4bit = (s8)m_state.A - (s8)value;
    setZeroFlag(result == 0);
    setHalfCarryFlag(result4bit >> 4);
    setCarryFlag(result >> 8);

    switch (m_mode)
    {
    case Mode::GameBoy:
        setSubtractFlag(1);
        break;
    case Mode::Intel8080:
        setSignFlag(result >> 7);
        setParityFlag((std::bitset<8>(m_state.A).count() % 2) == 0);
        break;
    }
}

void CPU8080::DECR(u8& reg)
{
    u8 tempBit = ~reg & 0x10;
    reg--;
    setHalfCarryFlag(((reg & 0x10) & tempBit) >> 4);
    setZeroFlag(reg == 0);

    switch (m_mode)
    {
    case Mode::GameBoy:
        setSubtractFlag(1);
        break;
    case Mode::Intel8080:
        setSignFlag(reg >> 7);
        setParityFlag((std::bitset<8>(reg).count() % 2) == 0);
        break;
    }
}

void CPU8080::DECRP(u16& reg)
{
    reg--;
}

void CPU8080::DECM()
{
    u8 value = load8(m_state.HL);
    u8 tempBit = ~value & 0x10;
    value--;
    setHalfCarryFlag(((value & 0x10) & tempBit) >> 4);
    setZeroFlag(value == 0);
    store8(m_state.HL, value);

    switch (m_mode)
    {
    case Mode::GameBoy:
        setSubtractFlag(1);
        break;
    case Mode::Intel8080:
        setSignFlag(value >> 7);
        setParityFlag((std::bitset<8>(value).count() % 2) == 0);
        break;
    }
}

void CPU8080::INCR(u8& reg)
{
    u8 tempBit = reg & 0x10;
    reg++;
    setHalfCarryFlag((~(reg & 0x10) & tempBit) >> 4);
    setZeroFlag(reg == 0);

    switch (m_mode)
    {
    case Mode::GameBoy:
        setSubtractFlag(0);
        break;
    case Mode::Intel8080:
        setSignFlag(reg >> 7);
        setParityFlag((std::bitset<8>(reg).count() % 2) == 0);
        break;
    }
}

void CPU8080::INCRP(u16& reg)
{
    reg++;
}

void CPU8080::INCM()
{
    u8 value = load8(m_state.HL);
    u8 tempBit = value & 0x10;
    value++;
    setHalfCarryFlag((~(value & 0x10) & tempBit) >> 4);
    setSignFlag(value >> 7);
    setZeroFlag(value == 0);
    setParityFlag((std::bitset<8>(value).count() % 2) == 0);
    store8(m_state.HL, value);
}

void CPU8080::RL(u8& reg)
{
    u8 temp = getCarryFlag();
    setCarryFlag(reg >> 7);
    reg <<= 1;
    reg |= temp;

    setZeroFlag(reg == 0);
}

void CPU8080::RRC()
{
    setCarryFlag(m_state.A & 1);
    m_state.A >>= 1;
    m_state.A |= getCarryFlag() << 7;
}

void CPU8080::RLC()
{
    setCarryFlag(m_state.A >> 7);
    m_state.A <<= 1;
    m_state.A |= getCarryFlag();
}

void CPU8080::RAR()
{
    u8 temp = getCarryFlag() << 7;
    setCarryFlag(m_state.A & 1);
    m_state.A >>= 1;
    m_state.A |= temp;
}

void CPU8080::RAL()
{
    u8 temp = getCarryFlag();
    setCarryFlag(m_state.A >> 7);
    m_state.A <<= 1;
    m_state.A |= temp;
}

void CPU8080::DAA()
{
    u16 value = m_state.A;

    if ((value & 0x0F) > 9 || getHalfCarryFlag())
        value += 0x06;

    if ((value >> 4) > 9 || getCarryFlag())
        value += 0x60;

    m_state.A = value;
    setCarryFlag(value >> 8);
    setSignFlag(m_state.A >> 7);
    setZeroFlag(m_state.A == 0);
    setParityFlag((std::bitset<8>(m_state.A).count() % 2) == 0);
}

void CPU8080::AND(u8 value)
{
    m_state.A &= value;
    setZeroFlag(m_state.A == 0);

    switch (m_mode)
    {
    case Mode::Intel8080:
        setParityFlag((std::bitset<8>(m_state.A).count() % 2) == 0);
        break;
    case Mode::GameBoy:
        setCarryFlag(0);
        setHalfCarryFlag(0);
        setSubtractFlag(1);
        break;
    }
}

void CPU8080::OR(u8 value)
{
    m_state.A |= value;
    setZeroFlag(m_state.A == 0);

    switch (m_mode)
    {
    case Mode::Intel8080:
        setParityFlag((std::bitset<8>(m_state.A).count() % 2) == 0);
        break;
    case Mode::GameBoy:
        setCarryFlag(0);
        setHalfCarryFlag(0);
        setSubtractFlag(0);
        break;
    }
}

void CPU8080::XOR(u8 value)
{
    m_state.A ^= value;
    setZeroFlag(m_state.A == 0);

    switch (m_mode)
    {
    case Mode::Intel8080:
        setParityFlag((std::bitset<8>(m_state.A).count() % 2) == 0);
        break;
    case Mode::GameBoy:
        setCarryFlag(0);
        setHalfCarryFlag(0);
        setSubtractFlag(0);
        break;
    }
}

void CPU8080::STC()
{
    setCarryFlag(1);
}

void CPU8080::CMC()
{
    setCarryFlag(~getCarryFlag());
}

void CPU8080::CMA()
{
    m_state.A = ~m_state.A;
}

void CPU8080::JMP(bool flag)
{
    u16 address = load16(m_state.PC);
    m_state.PC += 2;
    if (flag) {
        m_state.PC = address;
    }
}

void CPU8080::JR(bool flag)
{
    s8 offset = load8(m_state.PC++);
    if (flag) {
        m_state.PC += offset;
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

void CPU8080::RST(u8 vector)
{
    push16(m_state.PC);
    m_state.PC = vector * 8;
}
