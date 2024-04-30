#include "cpu.hpp"

#include <cassert>

namespace PSX {

    void CPU::reset()
    {
        m_cpuStatus.PC = 0xBFC00000;

        m_cop0Status.SR = 0;

        m_nextInstruction = 0;
        m_pendingLoad = { RegIndex{ 0 }, 0 };
    }

    void CPU::clock()
    {
        Instruction inst = m_nextInstruction;
        m_nextInstruction = load32(m_cpuStatus.PC);
        m_cpuStatus.PC += 4;

        setReg(m_pendingLoad.regIndex, m_pendingLoad.value);
        m_pendingLoad = { RegIndex{ 0 }, 0 };

        switch (inst.opcode())
        {
        case 0x00:
            switch (inst.subfn())
            {
            case 0x00: op_SLL(inst.regD(), inst.regT(), inst.shift()); break;

            case 0x08: op_JR(inst.regS()); break;

            case 0x20: op_ADD(inst.regD(), inst.regS(), getReg(inst.regT())); break;
            case 0x21: op_ADDU(inst.regD(), inst.regS(), getReg(inst.regT())); break;

            case 0x24: op_AND(inst.regD(), inst.regS(), getReg(inst.regT())); break;
            case 0x25: op_OR(inst.regD(), inst.regS(), getReg(inst.regT())); break;

            case 0x2B: op_SLTU(inst.regD(), getReg(inst.regS()), getReg(inst.regT())); break;
            default:
                assert(false && "Unhandled opcode!");
            }
            break;

        case 0x02: op_J(inst.imm_jump()); break;
        case 0x03: op_JAL(inst.imm_jump()); break;

        case 0x04: branch(getReg(inst.regS()) == getReg(inst.regT()), inst.imm_se() << 2); break;
        case 0x05: branch(getReg(inst.regS()) != getReg(inst.regT()), inst.imm_se() << 2); break;

        case 0x08: op_ADD(inst.regT(), inst.regS(), inst.imm_se()); break;
        case 0x09: op_ADDU(inst.regT(), inst.regS(), inst.imm_se()); break;

        case 0x0C: op_AND(inst.regT(), inst.regS(), inst.imm()); break;
        case 0x0D: op_OR(inst.regT(), inst.regS(), inst.imm()); break;

        case 0x0F: op_LUI(inst.regT(), inst.imm()); break;
        case 0x10:
            switch (inst.copfn())
            {
            case 0x00: op_MFC0(inst.regD(), inst.regT()); break;
            case 0x04: op_MTC0(inst.regD(), inst.regT()); break;
            default:
                assert(false && "Unhandled opcode!");
            }
            break;

        case 0x20: op_LB(inst.regT(), inst.regS(), inst.imm_se()); break;

        case 0x23: op_LW(inst.regT(), inst.regS(), inst.imm_se()); break;

        case 0x28: op_SB(inst.regT(), inst.regS(), inst.imm_se()); break;
        case 0x29: op_SH(inst.regT(), inst.regS(), inst.imm_se()); break;

        case 0x2B: op_SW(inst.regT(), inst.regS(), inst.imm_se()); break;
        default:
            assert(false && "Unhandled opcode!");
        }

        std::memcpy(m_cpuStatus.inputRegs, m_cpuStatus.outputRegs, REGISTER_COUNT * sizeof(u32));

        assert(getReg(RegIndex{ 0 }) == 0 && "GPR zero value was changed!");
    }

    CPU::CPU()
    {
        setReg(RegIndex{ 0 }, 0);
    }

    void CPU::setReg(RegIndex index, u32 value)
    {
        assert(index.i < REGISTER_COUNT && "Index out of bounds!");

        m_cpuStatus.outputRegs[index.i] = value;
        m_cpuStatus.outputRegs[0] = 0;
    }

    void CPU::branch(bool condition, u32 offset)
    {
        if (condition) {
            m_cpuStatus.PC += offset;
            m_cpuStatus.PC -= 4;
        }
    }

    void CPU::op_MFC0(RegIndex copIndex, RegIndex cpuIndex)
    {
        m_pendingLoad = { cpuIndex, m_cop0Status.regs[copIndex.i] };
    }

    void CPU::op_MTC0(RegIndex copIndex, RegIndex cpuIndex)
    {
        m_cop0Status.regs[copIndex.i] = getReg(cpuIndex);
    }

    void CPU::op_SLL(RegIndex d, RegIndex t, u32 shift)
    {
        setReg(d, getReg(t) << shift);
    }

    void CPU::op_OR(RegIndex target, RegIndex lhs, u32 rhs)
    {
        setReg(target, getReg(lhs) | rhs);
    }

    void CPU::op_AND(RegIndex target, RegIndex lhs, u32 rhs)
    {
        setReg(target, getReg(lhs) & rhs);
    }

    void CPU::op_J(u32 immediate)
    {
        m_cpuStatus.PC = (m_cpuStatus.PC & 0xF0000000) | immediate;
    }

    void CPU::op_JAL(u32 immediate)
    {
        setReg(RegIndex{ 31 }, m_cpuStatus.PC);
        op_J(immediate);
    }

    void CPU::op_JR(RegIndex s)
    {
        m_cpuStatus.PC = getReg(s);
    }

    void CPU::op_ADD(RegIndex t, RegIndex s, u32 rhs)
    {
        // TODO: check this code for egde cases
        s32 a = getReg(s);
        s32 b = (s32)rhs;
        s32 result = a + b;
        if (a > 0 && b > 0 && result < 0 ||
            a < 0 && b < 0 && result > 0)
            assert(false && "Overflow exception!");
        else
           setReg(t, result);
    }

    void CPU::op_ADDU(RegIndex t, RegIndex s, u32 rhs)
    {
        setReg(t, getReg(s) + rhs);
    }

    void CPU::op_LUI(RegIndex t, u32 immediate)
    {
        setReg(t, immediate << 16);
    }

    void CPU::op_SB(RegIndex t, RegIndex s, u32 immediate)
    {
        if (m_cop0Status.SR & 0x10000)
            return; // isolated cache bit is set

        store8(getReg(s) + immediate, getReg(t) & 0xFF);
    }

    void CPU::op_SH(RegIndex t, RegIndex s, u32 immediate)
    {
        if (m_cop0Status.SR & 0x10000)
            return; // isolated cache bit is set

        store16(getReg(s) + immediate, getReg(t) & 0xFFFF);
    }

    void CPU::op_SW(RegIndex t, RegIndex s, u32 immediate)
    {
        if (m_cop0Status.SR & 0x10000)
            return; // isolated cache bit is set

        store32(getReg(s) + immediate, getReg(t));
    }

    void CPU::op_LB(RegIndex t, RegIndex s, u32 immediate)
    {
        if (m_cop0Status.SR & 0x10000)
            return; // isolated cache bit is set

        s8 value = load8(getReg(s) + immediate);
        m_pendingLoad = { t, static_cast<u32>(value) };
    }

    void CPU::op_LW(RegIndex t, RegIndex s, u32 immediate)
    {
        if (m_cop0Status.SR & 0x10000)
            return; // isolated cache bit is set

        m_pendingLoad = { t, load32(getReg(s) + immediate) };
    }

    void CPU::op_SLTU(RegIndex d, u32 lhs, u32 rhs)
    {
        setReg(d, lhs < rhs);
    }

} // namespace PSX
