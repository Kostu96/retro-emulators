#include "cpu4040.hpp"

#include <cassert>

void CPU4040::reset()
{
    std::memset(m_regs, 0, REGS_SIZE);
    std::memset(m_stack, 0, STACK_SIZE * 2);
    SP = 0;
    ACC = 0;
    CY = 0;
}

void CPU4040::clock()
{
    u8 opcode = load8(getPC());
    incPC();

    switch (opcode)
    {
    case 0x20: FIM(m_regs + 0); break;
    case 0x22: FIM(m_regs + 1); break;
    case 0x24: FIM(m_regs + 2); break;
    case 0x26: FIM(m_regs + 3); break;
    case 0x28: FIM(m_regs + 4); break;
    case 0x2A: FIM(m_regs + 5); break;
    case 0x2C: FIM(m_regs + 6); break;
    case 0x2E: FIM(m_regs + 7); break;

    case 0x80: ADD(m_regs, 0x0); break;
    case 0x81: ADD(m_regs, 0x1); break;
    case 0x82: ADD(m_regs, 0x2); break;
    case 0x83: ADD(m_regs, 0x3); break;
    case 0x84: ADD(m_regs, 0x4); break;
    case 0x85: ADD(m_regs, 0x5); break;
    case 0x86: ADD(m_regs, 0x6); break;
    case 0x87: ADD(m_regs, 0x7); break;
    case 0x88: ADD(m_regs, 0x8); break;
    case 0x89: ADD(m_regs, 0x9); break;
    case 0x8A: ADD(m_regs, 0xA); break;
    case 0x8B: ADD(m_regs, 0xB); break;
    case 0x8C: ADD(m_regs, 0xC); break;
    case 0x8D: ADD(m_regs, 0xD); break;
    case 0x8E: ADD(m_regs, 0xE); break;
    case 0x8F: ADD(m_regs, 0xF); break;

    case 0xA0: LD(m_regs, 0x0); break;
    case 0xA1: LD(m_regs, 0x1); break;
    case 0xA2: LD(m_regs, 0x2); break;
    case 0xA3: LD(m_regs, 0x3); break;
    case 0xA4: LD(m_regs, 0x4); break;
    case 0xA5: LD(m_regs, 0x5); break;
    case 0xA6: LD(m_regs, 0x6); break;
    case 0xA7: LD(m_regs, 0x7); break;
    case 0xA8: LD(m_regs, 0x8); break;
    case 0xA9: LD(m_regs, 0x9); break;
    case 0xAA: LD(m_regs, 0xA); break;
    case 0xAB: LD(m_regs, 0xB); break;
    case 0xAC: LD(m_regs, 0xC); break;
    case 0xAD: LD(m_regs, 0xD); break;
    case 0xAE: LD(m_regs, 0xE); break;
    case 0xAF: LD(m_regs, 0xF); break;
    case 0xB0: XCH(m_regs, 0x0); break;
    case 0xB1: XCH(m_regs, 0x1); break;
    case 0xB2: XCH(m_regs, 0x2); break;
    case 0xB3: XCH(m_regs, 0x3); break;
    case 0xB4: XCH(m_regs, 0x4); break;
    case 0xB5: XCH(m_regs, 0x5); break;
    case 0xB6: XCH(m_regs, 0x6); break;
    case 0xB7: XCH(m_regs, 0x7); break;
    case 0xB8: XCH(m_regs, 0x8); break;
    case 0xB9: XCH(m_regs, 0x9); break;
    case 0xBA: XCH(m_regs, 0xA); break;
    case 0xBB: XCH(m_regs, 0xB); break;
    case 0xBC: XCH(m_regs, 0xC); break;
    case 0xBD: XCH(m_regs, 0xD); break;
    case 0xBE: XCH(m_regs, 0xE); break;
    case 0xBF: XCH(m_regs, 0xF); break;
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

void CPU4040::ADD(u8* regs, u8 idx)
{
    u8 value = regs[idx / 2];
    ACC += (idx % 2 ? value & 0xF : value >> 4) + CY;
    CY = ACC >> 4;
    ACC &= 0xF;
}

void CPU4040::FIM(u8* reg)
{
    *reg = load8(getPC());
    incPC();
}

void CPU4040::LD(u8* regs, u8 idx)
{
    u8 value = regs[idx / 2];
    ACC = idx % 2 ? value & 0xF : value >> 4;
}

void CPU4040::XCH(u8* regs, u8 idx)
{
    u8 temp = ACC;
    u8 value = regs[idx / 2];
    ACC = idx % 2 ? value & 0xF : value >> 4;
    regs[idx / 2] &= idx % 2 ? 0x0F : 0xF0;
    regs[idx / 2] |= idx % 2 ? temp : temp << 4;
}
