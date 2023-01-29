#include "cpu40xx.hpp"

#include <cassert>

void CPU40xx::reset()
{
    std::memset(m_regs, 0, REGS_SIZE);
    std::memset(m_stack, 0, STACK_SIZE * 2);
    SP = 0;
    ACC = 0;
    CY = 0;
    m_test = 0;
    SRCReg = 0;
    CMRAM = 0;
}

void CPU40xx::clock()
{
    u8 opcode = loadROM8(getPC());
    incPC();

    switch (opcode >> 4)
    {
    case 0x1: JCN(opcode & 0xF); break;
    case 0x2:
        if (opcode & 1)
            SRC(m_regs + (opcode & 0xE) / 2);
        else
            FIM(m_regs + (opcode & 0xE) / 2);
        break;
    case 0x3:
        if (opcode & 1)
            JIN(m_regs + (opcode & 0xE) / 2);
        else
            FIN(m_regs + (opcode & 0xE) / 2);
        break;
    case 0x4: JUN(opcode & 0xF); break;
    case 0x5: JMS(opcode & 0xF); break;
    case 0x6: INC(m_regs, opcode & 0xF); break;
    case 0x7: ISZ(m_regs, opcode & 0xF); break;
    case 0x8: ADD(m_regs, opcode & 0xF); break;
    case 0x9: SUB(m_regs, opcode & 0xF); break;
    case 0xA: LD(m_regs, opcode & 0xF); break;
    case 0xB: XCH(m_regs, opcode & 0xF); break;
    case 0xC: BBL(opcode & 0xF); break;
    case 0xD: LDM(opcode & 0xF); break;
    case 0xE:
        switch (opcode & 0xF)
        {
        case 0x0: WRM(); break;
        case 0x1: WMP(); break;
        case 0x2: WRR(); break;
        case 0x4: WRX(0); break;
        case 0x5: WRX(1); break;
        case 0x6: WRX(2); break;
        case 0x7: WRX(3); break;
        case 0x8: SBM(); break;
        case 0x9: RDM(); break;
        case 0xA: RDR(); break;
        case 0xB: ADM(); break;
        case 0xC: RDX(0); break;
        case 0xD: RDX(1); break;
        case 0xE: RDX(2); break;
        case 0xF: RDX(3); break;
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
    m_mode{ mode },
    REGS_SIZE{ mode == Mode::Intel4004 ? 8u : 12u },
    STACK_SIZE{ mode == Mode::Intel4004 ? 4u : 8u },
    m_regs{ new u8[REGS_SIZE] },
    m_stack{ new u16[STACK_SIZE] }
{}

CPU40xx::~CPU40xx()
{
    delete[] m_regs;
    delete[] m_stack;
}

u8 CPU40xx::loadROM8(u16 address) const
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

u8 CPU40xx::loadRAM4()
{
    u16 address = 0;
    address |= CMRAM >> 1;
    address |= SRCReg;

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

void CPU40xx::storeRAM4(u8 data)
{
    u16 address = 0;
    address |= CMRAM >> 1;
    address |= SRCReg;

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
    assert(stored && "Unhandled memory read");
}

void CPU40xx::ADD(const u8* regs, u8 idx)
{
    u8 value = regs[idx / 2];
    u8 temp = ACC + (idx % 2 ? value & 0xF : value >> 4) + CY;
    ACC = temp;
    CY = temp >> 4;
}

void CPU40xx::ADM()
{
    u8 temp = ACC + loadRAM4() + CY;
    ACC = temp;
    CY = temp >> 4;
}

void CPU40xx::BBL(u8 data)
{
    SP--;
    ACC = data;
}

void CPU40xx::CLB()
{
    ACC = 0;
    CY = 0;
}

void CPU40xx::CLC()
{
    CY = 0;
}

void CPU40xx::CMA()
{
    ACC = ~ACC;
}

void CPU40xx::CMC()
{
    CY = ~CY;
}

void CPU40xx::DAA()
{
    if (ACC > 9 || CY) {
        u8 temp = ACC + 6;
        ACC = temp;
        CY = temp >> 4;
    }
}

void CPU40xx::DAC()
{
    u8 temp = ACC + 0xF;
    ACC = temp;
    CY = temp >> 4;;
}

void CPU40xx::DCL()
{
    if (ACC == 0) CMRAM = 1;
    else CMRAM = ACC << 1;
}

void CPU40xx::FIM(u8* reg)
{
    *reg = loadROM8(getPC());
    incPC();
}

void CPU40xx::FIN(u8* reg)
{
    u16 addr = m_regs[0];
    if ((getPC() & 0xFF) == 0xFF) addr += 0x100;
    *reg = loadROM8(addr);
}

void CPU40xx::IAC()
{
    u8 temp = ACC + 1;
    ACC = temp;
    CY = temp >> 4;
}

void CPU40xx::INC(u8* regs, u8 idx)
{
    regs += idx / 2;
    u8 temp = (idx % 2 ? *regs & 0xF : *regs >> 4) + 1;
    temp &= 0xF;
    *regs &= idx % 2 ? 0xF0 : 0x0F;
    *regs |= idx % 2 ? temp : temp << 4;
}

void CPU40xx::ISZ(u8* regs, u8 idx)
{
    u8 address = loadROM8(getPC());
    incPC();

    regs += idx / 2;
    u8 temp = (idx % 2 ? *regs & 0xF : *regs >> 4) + 1;
    temp &= 0xF;
    *regs &= idx % 2 ? 0xF0 : 0x0F;
    *regs |= idx % 2 ? temp : temp << 4;

    if (temp)
    {
        if ((getPC() & 0xFFu) == 0xFE) m_stack[getSP()] += 2;
        m_stack[getSP()] &= 0x0300u;
        m_stack[getSP()] |= address;
    }
}

void CPU40xx::JCN(u8 condition)
{
    u8 address = loadROM8(getPC());
    incPC();

    u8 c1 = (condition >> 3) & 1;
    u8 c2 = (condition >> 2) & 1;
    u8 c3 = (condition >> 1) & 1;
    u8 c4 = (condition >> 0) & 1;

    u8 isACCZero = ACC == 0;
    u8 allTests = isACCZero & c2 | CY & c3 | !m_test & c4;
    bool shouldJump = !c1 & allTests | c1 & !allTests;

    if (shouldJump) {
        if ((getPC() & 0xFFu) == 0xFE) m_stack[getSP()] += 2;
        m_stack[getSP()] &= 0x0300u;
        m_stack[getSP()] |= address;
    }
}

void CPU40xx::JIN(const u8* reg)
{
    if ((getPC() & 0xFF) == 0xFF) incPC();
    m_stack[getSP()] &= 0xF00;
    m_stack[getSP()] |= m_regs[0];
}

void CPU40xx::JMS(u16 highNibble)
{
    u16 addr = (highNibble << 8) | loadROM8(getPC());
    incPC();
    SP++;
    m_stack[getSP()] = addr;
}

void CPU40xx::JUN(u16 highNibble)
{
    m_stack[getSP()] = (highNibble << 8) | loadROM8(getPC());
}

void CPU40xx::KBP()
{
    u8 temp = ACC & 0x0Fu;
    switch (temp)
    {
    case 0b0000:
    case 0b0001:
    case 0b0010: return;
    case 0b0100: ACC = 0b0011; return;
    case 0b1000: ACC = 0b0100; return;
    default:     ACC = 0b1111; return;
    }
}

void CPU40xx::LD(const u8* regs, u8 idx)
{
    u8 value = regs[idx / 2];
    ACC = idx % 2 ? value : value >> 4;
}

void CPU40xx::LDM(u8 data)
{
    ACC = data;
}

void CPU40xx::RAL()
{
    CY = ACC >> 3;
    ACC <<= 1;
    ACC |= CY;
}

void CPU40xx::RAR()
{
    CY = ACC;
    ACC >>= 1;
    ACC |= CY << 3;
}

void CPU40xx::RDM()
{
    ACC = loadRAM4();
}

void CPU40xx::RDR()
{
    ACC = m_readROMIO(ROMChip);
}

void CPU40xx::RDX(u8 charIdx)
{
    ACC = m_readRAMStatus((RAMChip << 4) | (RAMRegIdx << 2) | charIdx);
}

void CPU40xx::SBM()
{
    u8 value = ~loadRAM4() & 0xF;
    u8 temp = ACC + value + CY;
    ACC = temp;
    CY = temp >> 4;
}

void CPU40xx::SRC(const u8* reg)
{
    SRCReg = *reg;
}

void CPU40xx::STC()
{
    CY = 1;
}

void CPU40xx::SUB(const u8* regs, u8 idx)
{
    u8 value = ~regs[idx / 2];
    u8 temp = ACC + (idx % 2 ? value & 0xF : value >> 4) + CY;
    ACC = temp;
    CY = temp >> 4;
}

void CPU40xx::TCC()
{
    ACC = CY;
    CY = 0;
}

void CPU40xx::TCS()
{
    ACC = 9 + CY;
    CY = 0;
}

void CPU40xx::WMP()
{
    m_writeRAMOut(RAMChip, ACC);
}

void CPU40xx::WRM()
{
    storeRAM4(ACC);
}

void CPU40xx::WRR()
{
    m_writeROMIO(ROMChip, ACC);
}

void CPU40xx::WRX(u8 charIdx)
{
    m_writeRAMStatus((RAMChip << 4) | (RAMRegIdx << 2) | charIdx, ACC);
}

void CPU40xx::XCH(u8* regs, u8 idx)
{
    u8 temp = ACC;
    u8 value = regs[idx / 2];
    ACC = idx % 2 ? value : value >> 4;
    regs[idx / 2] &= idx % 2 ? 0xF0 : 0x0F;
    regs[idx / 2] |= idx % 2 ? temp : temp << 4;
}
