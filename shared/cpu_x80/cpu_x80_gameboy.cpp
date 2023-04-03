#include "cpu_x80_base.inl"

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

template <>
void CPUx80<CPUx80Mode::GameBoy>::clock()
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

template <>
u8 CPUx80<CPUx80Mode::GameBoy>::getCarryFlag()
{
    return m_state.F.Carry;
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::setCarryFlag(u8 value)
{
    m_state.F.Carry = value;
}

template <>
u8 CPUx80<CPUx80Mode::GameBoy>::getHalfCarryFlag()
{
    return m_state.F.HalfCarry;
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::setHalfCarryFlag(u8 value)
{
    m_state.F.HalfCarry = value;
}

template <>
u8 CPUx80<CPUx80Mode::GameBoy>::getSubtractFlag()
{
    return m_state.F.Subtract;
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::setSubtractFlag(u8 value)
{
    m_state.F.Subtract = value;
}

template <>
u8 CPUx80<CPUx80Mode::GameBoy>::getZeroFlag()
{
    return m_state.F.Zero;
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::setZeroFlag(u8 value)
{
    m_state.F.Zero = value;
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::prefixInstruction(u8 opcode)
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

template <>
void CPUx80<CPUx80Mode::GameBoy>::ADD(u8 value)
{
    u16 result;
    u8 result4bit;
    add8(value, 0, result, result4bit);

    setZeroFlag(m_state.A == 0);
    setHalfCarryFlag(result4bit >> 4);
    setCarryFlag(result >> 8);
    setSubtractFlag(0);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::ADC(u8 value)
{
    u16 result;
    u8 result4bit;
    add8(value, getCarryFlag(), result, result4bit);

    setZeroFlag(m_state.A == 0);
    setHalfCarryFlag(result4bit >> 4);
    setCarryFlag(result >> 8);
    setSubtractFlag(0);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::ADDHL(u16 value)
{
    u32 result;
    u16 result12bit;
    add16(value, result, result12bit);
    
    setCarryFlag(result >> 16);
    setHalfCarryFlag(result12bit >> 12);
    setSubtractFlag(0);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::SUB(u8 value)
{
    u16 result;
    u8 result4bit;
    subtract(value, 0, result, result4bit);
    
    setZeroFlag(m_state.A == 0);
    setHalfCarryFlag(result4bit >> 4);
    setCarryFlag(result >> 8);
    setSubtractFlag(1);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::SBB(u8 value)
{
    u16 result;
    u8 result4bit;
    subtract(value, getCarryFlag(), result, result4bit);

    setZeroFlag(m_state.A == 0);
    setHalfCarryFlag(result4bit >> 4);
    setCarryFlag(result >> 8);
    setSubtractFlag(1);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::CMP(u8 value)
{
    u16 result;
    u8 result4bit;
    compare(value, result, result4bit);
    
    setZeroFlag(result == 0);
    setHalfCarryFlag(result4bit >> 4);
    setCarryFlag(result >> 8);
    setSubtractFlag(1);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::DAA()
{
    u8 u = 0;
    u8 fc = 0;

    if (getHalfCarryFlag() || (!getSubtractFlag() && (m_state.A & 0xF) > 9)) {
        u = 6;
    }
    if (getCarryFlag() || (!getSubtractFlag() && m_state.A > 0x99)) {
        u |= 0x60;
        fc = 1;
    }

    m_state.A += getSubtractFlag() ? -u : u;

    setCarryFlag(fc);
    setHalfCarryFlag(0);
    setZeroFlag(m_state.A == 0);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::RLCA()
{
    u8 newCarry;
    rotateLeft(m_state.A, newCarry);

    setCarryFlag(newCarry);
    setZeroFlag(0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::RAL()
{
    u8 newCarry;
    rotateLeftWithCarry(m_state.A, getCarryFlag(), newCarry);
    
    setCarryFlag(newCarry);
    setZeroFlag(0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::RRCA()
{
    u8 newCarry;
    rotateRight(m_state.A, newCarry);

    setCarryFlag(newCarry);
    setZeroFlag(0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::RAR()
{
    u8 newCarry;
    rotateRightWithCarry(m_state.A, getCarryFlag(), newCarry);
    
    setCarryFlag(newCarry);
    setZeroFlag(0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::RLC(u8& reg)
{
    u8 newCarry;
    rotateLeft(reg, newCarry);

    setCarryFlag(newCarry);
    setZeroFlag(reg == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::RLCM()
{
    u8 value = load8(m_state.HL);
    RLC(value);
    store8(m_state.HL, value);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::RL(u8& reg)
{
    u8 newCarry;
    rotateLeftWithCarry(reg, getCarryFlag(), newCarry);
    
    setCarryFlag(newCarry);
    setZeroFlag(reg == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::RLM()
{
    u8 value = load8(m_state.HL);
    RL(value);
    store8(m_state.HL, value);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::RRC(u8& reg)
{
    u8 newCarry;
    rotateRight(reg, newCarry);

    setCarryFlag(newCarry);
    setZeroFlag(reg == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::RRCM()
{
    u8 value = load8(m_state.HL);
    RRC(value);
    store8(m_state.HL, value);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::RR(u8& reg)
{
    u8 newCarry;
    rotateRightWithCarry(reg, getCarryFlag(), newCarry);
    
    setCarryFlag(newCarry);
    setZeroFlag(reg == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::RRM()
{
    u8 value = load8(m_state.HL);
    RR(value);
    store8(m_state.HL, value);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::SLA(u8& reg)
{
    u8 newCarry;
    shiftLeftArithmetic(reg, newCarry);

    setCarryFlag(newCarry);
    setZeroFlag(reg == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::SLAM()
{
    u8 value = load8(m_state.HL);
    SLA(value);
    store8(m_state.HL, value);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::SRA(u8& reg)
{
    u8 newCarry;
    shiftRightArithmetic(reg, newCarry);

    setCarryFlag(newCarry);
    setZeroFlag(reg == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::SRAM()
{
    u8 value = load8(m_state.HL);
    SRA(value);
    store8(m_state.HL, value);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::SRL(u8& reg)
{
    u8 newCarry;
    shiftRightLogic(reg, newCarry);

    setCarryFlag(newCarry);
    setZeroFlag(reg == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::SRLM()
{
    u8 value = load8(m_state.HL);
    SRL(value);
    store8(m_state.HL, value);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::SWAP(u8& reg)
{
    u8 l = reg & 0x0F;
    u8 h = reg & 0xF0;
    reg = (l << 4) | (h >> 4);
    
    setZeroFlag(reg == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);
    setCarryFlag(0);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::SWAPM()
{
    u8 value = load8(m_state.HL);
    SWAP(value);
    store8(m_state.HL, value);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::BIT(u8 value, u8 bit)
{
    setZeroFlag(!((value >> bit) & 1));
    setSubtractFlag(0);
    setHalfCarryFlag(1);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::RES(u8& reg, u8 bit)
{
    u8 mask = ~(1 << bit);
    reg &= mask;
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::RESM(u8 bit)
{
    u8 value = load8(m_state.HL);
    RES(value, bit);
    store8(m_state.HL, value);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::SET(u8& reg, u8 bit)
{
    u8 mask = 1 << bit;
    reg |= mask;
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::SETM(u8 bit)
{
    u8 value = load8(m_state.HL);
    SET(value, bit);
    store8(m_state.HL, value);
}

template class CPUx80<CPUx80Mode::GameBoy>;
