#include "cpu4040.hpp"

#include <cassert>

void CPU4040::reset()
{
    std::memset(m_regs, 0, REGS_SIZE);
    std::memset(m_stack, 0, STACK_SIZE * 2);
    SP = 0;
    ACC = 0;
    CY = 0;
    m_test = 0;
    RAMAddr = 0;
}

void CPU4040::clock()
{
    u8 opcode = load8(getPC());
    incPC();

    switch (opcode >> 4)
    {
    case 0x1: JCM(opcode & 0xF); break;
    case 0x2:
        if (opcode & 1)
            SRC(m_regs + (opcode & 0xE) / 2);
        else
            FIM(m_regs + (opcode & 0xE) / 2);
        break;

    case 0x4: JUN(opcode & 0xF); break;
    case 0x5: JMS(opcode & 0xF); break;
    case 0x6: INC(m_regs, opcode & 0xF); break;

    case 0x8: ADD(m_regs, opcode & 0xF); break;

    case 0xA: LD(m_regs, opcode & 0xF); break;
    case 0xB: XCH(m_regs, opcode & 0xF); break;
    case 0xC: BBL(opcode & 0xF); break;
    case 0xD: LDM(opcode & 0xF); break;
    case 0xE:
        switch (opcode & 0xF)
        {
        case 0x1: WMP(); break;
        case 0xA: RDR(); break;
        default:
            assert(false && "Unhandled instruction");
        }
        break;
    case 0xF:
        switch (opcode & 0xF)
        {
        case 0x0: CLB(); break;
        case 0x1: CLC(); break;
        case 0x5: RAL(); break;
        case 0x6: RAR(); break;
        case 0x8: DAC(); break;
        default:
            assert(false && "Unhandled instruction");
        }
        break;
    default:
        assert(false && "Unhandled instruction");
    }
}

CPU4040::CPU4040(Mode mode) :
    m_mode{ mode },
    REGS_SIZE{ mode == Mode::Intel4004 ? 8u : 12u },
    STACK_SIZE{ mode == Mode::Intel4004 ? 4u : 8u },
    m_regs{ new u8[REGS_SIZE] },
    m_stack{ new u16[STACK_SIZE] }
{}

CPU4040::~CPU4040()
{
    delete[] m_regs;
    delete[] m_stack;
}

u8 CPU4040::load8(u16 address) const
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

void CPU4040::ADD(const u8* regs, u8 idx)
{
    u8 value = regs[idx / 2];
    ACC += (idx % 2 ? value & 0xF : value >> 4) + CY;
    CY = ACC >> 4;
    ACC &= 0xF;
}

void CPU4040::BBL(u8 data)
{
    SP--;
    ACC = data;
}

void CPU4040::CLB()
{
    ACC = 0;
    CY = 0;
}

void CPU4040::CLC()
{
    CY = 0;
}

void CPU4040::DAC()
{
    ACC += 0xF;
    CY = ACC >> 4;
    ACC &= 0xF;
}

void CPU4040::FIM(u8* reg)
{
    *reg = load8(getPC());
    incPC();
}

void CPU4040::INC(u8* regs, u8 idx)
{
    regs[idx / 2] += idx % 2 ? 0x1 : 0x10;
}

void CPU4040::JCM(u8 condition)
{
    u8 address = load8(getPC());
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

void CPU4040::JMS(u16 highNibble)
{
    u16 addr = (highNibble << 8) | load8(getPC());
    incPC();
    SP++;
    m_stack[getSP()] = addr;
}

void CPU4040::JUN(u16 highNibble)
{
    m_stack[getSP()] = (highNibble << 8) | load8(getPC());
}

void CPU4040::LD(const u8* regs, u8 idx)
{
    u8 value = regs[idx / 2];
    ACC = idx % 2 ? value & 0xF : value >> 4;
}

void CPU4040::LDM(u8 data)
{
    ACC = data;
}

void CPU4040::RAL()
{
    CY = ACC >> 3;
    ACC <<= 1;
    ACC &= 0xE;
    ACC |= CY;
}

void CPU4040::RAR()
{
    CY = ACC & 1;
    ACC >>= 1;
    ACC |= CY << 3;
}

void CPU4040::RDR()
{
    ACC = m_readROMIO(RAMAddr);
}

void CPU4040::SRC(const u8* reg)
{
    RAMAddr = *reg;
}

void CPU4040::WMP()
{
    m_writeRAMOut(RAMAddr, ACC);
}

void CPU4040::XCH(u8* regs, u8 idx)
{
    u8 temp = ACC;
    u8 value = regs[idx / 2];
    ACC = idx % 2 ? value & 0xF : value >> 4;
    regs[idx / 2] &= idx % 2 ? 0xF0 : 0x0F;
    regs[idx / 2] |= idx % 2 ? temp : temp << 4;
}
