#include "cpu.hpp"

#include <cassert>

namespace PSX {

    void CPU::reset()
    {
        m_SR = 0;

        m_PC = 0xBFC00000;
    }

    void CPU::clock()
    {
        static Instruction next_instruction = 0;
        Instruction inst = next_instruction;
        next_instruction = load32(m_PC);
        m_PC += 4;

        switch (inst.opcode())
        {
        case 0x00:
            switch (inst.subfn())
            {
            case 0x00: op_SLL(inst.regD(), inst.regT(), inst.shift()); break;

            case 0x25: op_OR(inst.regD(), inst.regS(), m_regs[inst.regT()]); break;
            default:
                assert(false && "Unhandled opcode!");
            }
            break;

        case 0x02: op_J(inst.imm_jump()); break;

        case 0x09: op_ADDIU(inst.regT(), inst.regS(), inst.imm_se()); break;

        case 0x0D: op_OR(inst.regT(), inst.regS(), inst.imm()); break;

        case 0x0F: op_LUI(inst.regT(), inst.imm()); break;
        case 0x10:
            switch (inst.copfn())
            {
            case 0x04: op_MTC0(inst.regD(), inst.regT()); break;
            default:
                assert(false && "Unhandled opcode!");
            }
            break;

        case 0x2B: op_SW(inst.regT(), inst.regS(), inst.imm_se()); break;
        default:
            assert(false && "Unhandled opcode!");
        }

        assert(m_regs[0] == 0 && "GPR zero value was changed!");
    }

    CPU::CPU()
    {
        m_regs[0] = 0;
    }

    void CPU::setReg(u32 index, u32 value)
    {
        assert(index < 32 && "Index out of bounds!");

        m_regs[index] = value;
        m_regs[0] = 0;
    }

    void CPU::op_MTC0(u32 copIndex, u32 cpuIndex)
    {
        m_copRegs[copIndex] = m_regs[cpuIndex];
    }

    void CPU::op_SLL(u32 dIndex, u32 tIndex, u32 shift)
    {
        setReg(dIndex, m_regs[tIndex] << shift);
    }

    void CPU::op_OR(u32 targetIndex, u32 lhsIndex, u32 rhsValue)
    {
        setReg(targetIndex, m_regs[lhsIndex] | rhsValue);
    }

    void CPU::op_J(u32 immediate)
    {
        m_PC = (m_PC & 0xF0000000) | immediate;
    }

    void CPU::op_ADDIU(u32 tIndex, u32 sIndex, u32 immediate)
    {
        setReg(tIndex, m_regs[sIndex] + immediate);
    }

    void CPU::op_LUI(u32 index, u32 immediate)
    {
        setReg(index, immediate << 16);
    }

    void CPU::op_SW(u32 tIndex, u32 sIndex, u32 immediate)
    {
        if (m_SR & 0x10000)
            return; // isolated cache bit is set

        store32(m_regs[sIndex] + immediate, m_regs[tIndex]);
    }

} // namespace PSX
