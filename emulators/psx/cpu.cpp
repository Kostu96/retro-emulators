#include "cpu.hpp"

#include <cassert>

namespace PSX {

    void CPU::reset()
    {
        m_cpuStatus.PC = 0xBFC00000;

        m_cop0Status.SR = 0;

        m_nextInstruction = 0;
    }

    void CPU::clock()
    {
        Instruction inst = m_nextInstruction;
        m_nextInstruction = load32(m_cpuStatus.PC);
        m_cpuStatus.PC += 4;

        switch (inst.opcode())
        {
        case 0x00:
            switch (inst.subfn())
            {
            case 0x00: op_SLL(inst.regD(), inst.regT(), inst.shift()); break;

            case 0x25: op_OR(inst.regD(), inst.regS(), m_cpuStatus.regs[inst.regT()]); break;
            default:
                assert(false && "Unhandled opcode!");
            }
            break;

        case 0x02: op_J(inst.imm_jump()); break;

        case 0x05: branch(inst.regS() != inst.regT(), inst.imm_se() << 2); break;

        case 0x08: op_ADDI(inst.regT(), inst.regS(), inst.imm_se()); break;
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

        assert(m_cpuStatus.regs[0] == 0 && "GPR zero value was changed!");
    }

    CPU::CPU()
    {
        m_cpuStatus.regs[0] = 0;
    }

    void CPU::setReg(u32 index, u32 value)
    {
        assert(index < 32 && "Index out of bounds!");

        m_cpuStatus.regs[index] = value;
        m_cpuStatus.regs[0] = 0;
    }

    void CPU::branch(bool condition, u32 offset)
    {
        if (condition) {
            m_cpuStatus.PC += offset;
            m_cpuStatus.PC -= 4;
        }
    }

    void CPU::op_MTC0(u32 copIndex, u32 cpuIndex)
    {
        m_cop0Status.regs[copIndex] = m_cpuStatus.regs[cpuIndex];
    }

    void CPU::op_SLL(u32 dIndex, u32 tIndex, u32 shift)
    {
        setReg(dIndex, m_cpuStatus.regs[tIndex] << shift);
    }

    void CPU::op_OR(u32 targetIndex, u32 lhsIndex, u32 rhsValue)
    {
        setReg(targetIndex, m_cpuStatus.regs[lhsIndex] | rhsValue);
    }

    void CPU::op_J(u32 immediate)
    {
        m_cpuStatus.PC = (m_cpuStatus.PC & 0xF0000000) | immediate;
    }

    void CPU::op_ADDI(u32 tIndex, u32 sIndex, u32 immediate)
    {
        // TODO: check this code for egde cases
        s32 a = m_cpuStatus.regs[sIndex];
        s32 b = (s32)immediate;
        s32 result = a + b;
        if (a > 0 && b > 0 && result < 0 ||
            a < 0 && b < 0 && result > 0)
            assert(false && "Overflow exception!");
        else
           setReg(tIndex, result);
    }

    void CPU::op_ADDIU(u32 tIndex, u32 sIndex, u32 immediate)
    {
        setReg(tIndex, m_cpuStatus.regs[sIndex] + immediate);
    }

    void CPU::op_LUI(u32 index, u32 immediate)
    {
        setReg(index, immediate << 16);
    }

    void CPU::op_SW(u32 tIndex, u32 sIndex, u32 immediate)
    {
        if (m_cop0Status.SR & 0x10000)
            return; // isolated cache bit is set

        store32(m_cpuStatus.regs[sIndex] + immediate, m_cpuStatus.regs[tIndex]);
    }

} // namespace PSX
