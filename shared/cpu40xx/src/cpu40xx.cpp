#include "cpu40xx/cpu40xx.hpp"

#include <cassert>
#include <cstring>

void CPU40xx::reset() {
    memset(m_state.regs.data(), 0, m_state.regs.size());
    memset(m_state.stack.data(), 0, m_state.stack.size() * sizeof(u16));
    m_state.SP = 0;
    m_state.ACC = 0;
    m_state.CY = 0;
    m_state.test = 0;
    m_state.CMRAM = 1;
}

void CPU40xx::clock() {
    u8 opcode = loadROM8(getPC());
    incPC();

    switch (opcode >> 4)
    {
    case 0x0: break;
    case 0x1: JCN(opcode & 0xF); break;
    case 0x2: (opcode & 1) ? SRC(opcode & 0xE) : FIM(opcode & 0xE); break;
    case 0x3: (opcode & 1) ? JIN(opcode & 0xE) : FIN(opcode & 0xE); break;
    case 0x4: JUN(opcode & 0xF); break;
    case 0x5: JMS(opcode & 0xF); break;
    case 0x6: INC(opcode & 0xF); break;
    case 0x7: ISZ(opcode & 0xF); break;
    case 0x8: ADD(opcode & 0xF); break;
    case 0x9: SUB(opcode & 0xF); break;
    case 0xA: LD(opcode & 0xF); break;
    case 0xB: XCH(opcode & 0xF); break;
    case 0xC: BBL(opcode & 0xF); break;
    case 0xD: LDM(opcode & 0xF); break;
    case 0xE:
        switch (opcode & 0xF)
        {
        case 0x0: WRM(); break;
        case 0x1: WMP(); break;
        case 0x2: WRR(); break;
        case 0x4: WRx(0); break;
        case 0x5: WRx(1); break;
        case 0x6: WRx(2); break;
        case 0x7: WRx(3); break;
        case 0x8: SBM(); break;
        case 0x9: RDM(); break;
        case 0xA: RDR(); break;
        case 0xB: ADM(); break;
        case 0xC: RDx(0); break;
        case 0xD: RDx(1); break;
        case 0xE: RDx(2); break;
        case 0xF: RDx(3); break;
        default:
            assert(false && "Unhandled instruction");
        }
        break;
    case 0xF:
        switch (opcode & 0xF)
        {
        case 0x0: CLB(); break;
        case 0x1: CLC(); break;
        case 0x2: IAC(); break;
        case 0x3: CMC(); break;
        case 0x4: CMA(); break;
        case 0x5: RAL(); break;
        case 0x6: RAR(); break;
        case 0x7: TCC(); break;
        case 0x8: DAC(); break;
        case 0x9: TCS(); break;
        case 0xA: STC(); break;
        case 0xB: DAA(); break;
        case 0xC: KBP(); break;
        case 0xD: DCL(); break;
        default:
            assert(false && "Unhandled instruction");
        }
        break;
    default:
        assert(false && "Unhandled instruction");
    }
}

CPU40xx::CPU40xx(Mode mode) :
    m_mode{ mode } {
    m_state.regs = std::vector<u8>(mode == Mode::Intel4004 ? 16u : 24u);
    m_state.stack = std::vector<u16>(mode == Mode::Intel4004 ? 4u : 8u);
}

void CPU40xx::ADD(u8 idx) {
    u8 temp = m_state.ACC + m_state.regs[idx] + m_state.CY;
    m_state.ACC = temp;
    m_state.CY = temp >> 4;
}

void CPU40xx::ADM() {
    u8 temp = m_state.ACC + loadRAM8(m_state.CMRAM) + m_state.CY;
    m_state.ACC = temp;
    m_state.CY = temp >> 4;
}

void CPU40xx::BBL(u8 data) {
    m_state.ACC = data;
    decSP();
}

void CPU40xx::CLB() {
    m_state.ACC = 0;
    m_state.CY = 0;
}

void CPU40xx::CLC() {
    m_state.CY = 0;
}

void CPU40xx::CMA() {
    m_state.ACC = ~m_state.ACC;
}

void CPU40xx::CMC() {
    m_state.CY = ~m_state.CY;
}

void CPU40xx::DAA() {
    if (m_state.ACC > 9 || m_state.CY) {
        u8 temp = m_state.ACC + 6;
        m_state.ACC = temp;
        m_state.CY = temp >> 4;
    }
}

void CPU40xx::DAC() {
    u8 temp = m_state.ACC + 0xF;
    m_state.ACC = temp;
    m_state.CY = temp >> 4;;
}

void CPU40xx::DCL() {
    if ((m_state.ACC & 0b111) == 0) m_state.CMRAM = 1;
    else m_state.CMRAM = m_state.ACC << 1;
}

void CPU40xx::FIM(u8 idx) {
    u8 value = loadROM8(getPC());
    incPC();

    m_state.regs[idx] = value & 0xF;
    m_state.regs[idx + 1] = value >> 4;
}

void CPU40xx::FIN(u8 idx) {
    u16 address = (getPC() & 0xF00) | (m_state.regs[1] << 4) | m_state.regs[0];
    u8 value = loadROM8(address);
    m_state.regs[idx] = value & 0xF;
    m_state.regs[idx + 1] = value >> 4;
}

void CPU40xx::IAC() {
    u8 temp = m_state.ACC + 1;
    m_state.ACC = temp;
    m_state.CY = temp >> 4;
}

void CPU40xx::INC(u8 idx) {
    m_state.regs[idx]++;
    m_state.regs[idx] &= 0xF;
}

void CPU40xx::ISZ(u8 idx) {
    u8 address = loadROM8(getPC());
    incPC();

    m_state.regs[idx]++;
    m_state.regs[idx] &= 0xF;

    if (m_state.regs[idx] != 0) {
        getPC() &= 0xF00u;
        getPC() |= address;
    }
}

void CPU40xx::JCN(u8 condition) {
    u8 address = loadROM8(getPC());
    incPC();

    u8 c1 = (condition >> 3) & 1;
    u8 c2 = (condition >> 2) & 1;
    u8 c3 = (condition >> 1) & 1;
    u8 c4 = (condition >> 0) & 1;

    u8 isACCZero = m_state.ACC == 0;
    u8 allTests = (isACCZero & c2) | (m_state.CY & c3) | (~m_state.test & c4);

    if (c1 ^ allTests) {
        getPC() &= 0xF00;
        getPC() |= address;
    }
}

void CPU40xx::JIN(u8 idx) {
    u8 address = (m_state.regs[idx + 1] << 4) | m_state.regs[idx];
    getPC() &= 0xF00;
    getPC() |= address;
}

void CPU40xx::JMS(u16 highNibble) {
    u16 addr = (highNibble << 8) | loadROM8(getPC());
    incPC();
    incSP();
    getPC() = addr;
}

void CPU40xx::JUN(u16 highNibble) {
    getPC() = (highNibble << 8) | loadROM8(getPC());
}

void CPU40xx::KBP() {
    switch (m_state.ACC)
    {
    case 0b0000:
    case 0b0001:
    case 0b0010: return;
    case 0b0100: m_state.ACC = 0b0011; return;
    case 0b1000: m_state.ACC = 0b0100; return;
    default:     m_state.ACC = 0b1111; return;
    }
}

void CPU40xx::LD(u8 idx) {
    m_state.ACC = m_state.regs[idx];
}

void CPU40xx::LDM(u8 data) {
    m_state.ACC = data;
}

void CPU40xx::RAL() {
    u8 carry = m_state.CY;
    m_state.CY = m_state.ACC >> 3;
    m_state.ACC <<= 1;
    m_state.ACC |= carry;
}

void CPU40xx::RAR() {
    u8 carry = m_state.CY;
    m_state.CY = m_state.ACC & 1;
    m_state.ACC >>= 1;
    m_state.ACC |= carry << 3;
}

void CPU40xx::RDM() {
    m_state.ACC = loadRAM8(m_state.CMRAM);
}

void CPU40xx::RDR() {
    m_state.ACC = loadIO8(m_state.ROMChip) & 0xF;
}

void CPU40xx::RDx(u8 charIdx) {
    m_state.ACC = loadStatus8((m_state.RAMChip << 4) | (m_state.RAMRegIdx << 2) | charIdx) & 0xF;
}

void CPU40xx::SBM() {
    u8 value = ~loadRAM8(m_state.CMRAM) & 0xF;
    u8 temp = m_state.ACC + value + m_state.CY;
    m_state.ACC = temp;
    m_state.CY = temp >> 4;
}

void CPU40xx::SRC(u8 idx) {
    u8 value = (m_state.regs[idx + 1] << 4) | m_state.regs[idx];
    storeSRCReg(m_state.CMRAM, value);
}

void CPU40xx::STC() {
    m_state.CY = 1;
}

void CPU40xx::SUB(u8 idx) {
    u8 temp = m_state.ACC + (~m_state.regs[idx] & 0xF) + m_state.CY;
    m_state.ACC = temp;
    m_state.CY = temp >> 4;
}

void CPU40xx::TCC() {
    m_state.ACC = m_state.CY;
    m_state.CY = 0;
}

void CPU40xx::TCS() {
    m_state.ACC = 9 + m_state.CY;
    m_state.CY = 0;
}

void CPU40xx::WMP() {
    storeIO8(m_state.RAMChip, m_state.ACC);
}

void CPU40xx::WRM() {
    storeRAM8(m_state.CMRAM, m_state.ACC);
}

void CPU40xx::WRR() {
    storeIO8(m_state.ROMChip, m_state.ACC);
}

void CPU40xx::WRx(u8 charIdx) {
    storeStatus8((m_state.RAMChip << 4) | (m_state.RAMRegIdx << 2) | charIdx, m_state.ACC);
}

void CPU40xx::XCH(u8 idx) {
    u8 temp = m_state.ACC;
    m_state.ACC = m_state.regs[idx];
    m_state.regs[idx] = temp;
}
