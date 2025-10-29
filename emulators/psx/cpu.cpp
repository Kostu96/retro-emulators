#include "cpu.hpp"

#include <cassert>
#include <cstring>

namespace PSX {

    void CPU::reset()
    {
        m_cpuStatus.PC = 0xBFC00000;
        m_nextPC = 0xBFC00004;

        m_cop0Status.SR.value = 0;

        m_pendingLoad = { RegIndex{ 0 }, 0 };
        m_isBranch = false;
        m_isBranchDelaySlot = false;
    }

    void CPU::clock()
    {
        m_currentPC = m_cpuStatus.PC;
        if (m_currentPC % 4 != 0) {
            exception(Exception::LoadAddressError);
            return;
        }

        CPUInstruction inst = load32(m_cpuStatus.PC);
        m_cpuStatus.PC = m_nextPC;
        m_nextPC += 4;

        setReg(m_pendingLoad.regIndex, m_pendingLoad.value);
        m_pendingLoad = { RegIndex{ 0 }, 0 };

        m_isBranchDelaySlot = m_isBranch;
        m_isBranch = false;

        switch (inst.opcode())
        {
        case 0x00:
            switch (inst.subfn())
            {
            case 0x00: op_SLL(inst.regD(), inst.regT(), inst.shift()); break;

            case 0x02: op_SRL(inst.regD(), inst.regT(), inst.shift()); break;
            case 0x03: op_SRA(inst.regD(), inst.regT(), inst.shift()); break;
            case 0x04: op_SLL(inst.regD(), inst.regT(), getReg(inst.regS()) & 0x1F); break; // SLLV

            case 0x08: op_JR(inst.regS()); break;
            case 0x09: op_JALR(inst.regD(), inst.regS()); break;

            case 0x0C: op_SYSCALL(); break;

            case 0x10: op_MFHI(inst.regD()); break;
            case 0x11: op_MTHI(inst.regS()); break;
            case 0x12: op_MFLO(inst.regD()); break;
            case 0x13: op_MTLO(inst.regS()); break;

            case 0x1A: op_DIV(getReg(inst.regS()), getReg(inst.regT())); break;
            case 0x1B: op_DIVU(getReg(inst.regS()), getReg(inst.regT())); break;

            case 0x20: op_ADD(inst.regD(), inst.regS(), getReg(inst.regT())); break;
            case 0x21: op_ADDU(inst.regD(), inst.regS(), getReg(inst.regT())); break;

            case 0x23: op_SUBU(inst.regD(), inst.regS(), getReg(inst.regT())); break;
            case 0x24: op_AND(inst.regD(), inst.regS(), getReg(inst.regT())); break;
            case 0x25: op_OR(inst.regD(), inst.regS(), getReg(inst.regT())); break;

            case 0x2A: op_SLT(inst.regD(), getReg(inst.regS()), getReg(inst.regT())); break;
            case 0x2B: op_SLTU(inst.regD(), getReg(inst.regS()), getReg(inst.regT())); break;
            default:
                assert(false && "Unhandled opcode!");
            }
            break;
        case 0x01: op_BXX(inst.regS(), inst.imm_se_jump(), inst.word); break;
        case 0x02: op_J(inst.imm_jump()); break;
        case 0x03: op_JAL(inst.imm_jump()); break;
        case 0x04: branch(getReg(inst.regS()) == getReg(inst.regT()), inst.imm_se_jump()); break; // BEQ
        case 0x05: branch(getReg(inst.regS()) != getReg(inst.regT()), inst.imm_se_jump()); break; // BNE
        case 0x06: branch(static_cast<s32>(getReg(inst.regS())) <= 0, inst.imm_se_jump()); break; // BLEZ
        case 0x07: branch(static_cast<s32>(getReg(inst.regS())) > 0, inst.imm_se_jump()); break; // BGTZ
        case 0x08: op_ADD(inst.regT(), inst.regS(), inst.imm_se()); break; // ADDI
        case 0x09: op_ADDU(inst.regT(), inst.regS(), inst.imm_se()); break; // ADDIU
        case 0x0A: op_SLT(inst.regT(), getReg(inst.regS()), inst.imm_se()); break; // SLTI
        case 0x0B: op_SLTU(inst.regT(), getReg(inst.regS()), inst.imm_se()); break; // SLTIU
        case 0x0C: op_AND(inst.regT(), inst.regS(), inst.imm()); break; // ANDI
        case 0x0D: op_OR(inst.regT(), inst.regS(), inst.imm()); break; // ORI

        case 0x0F: op_LUI(inst.regT(), inst.imm()); break;
        case 0x10:
            switch (inst.copfn())
            {
            case 0x00: op_MFC0(inst.regD(), inst.regT()); break;
            case 0x04: op_MTC0(inst.regD(), inst.regT()); break;
            case 0x10: {
                assert(inst.subfn() == 0x10);
                op_RFE();
            } break;
            default:
                assert(false && "Unhandled opcode!");
            }
            break;

        case 0x20: op_LB(inst.regT(), inst.regS(), inst.imm_se()); break;

        case 0x23: op_LW(inst.regT(), inst.regS(), inst.imm_se()); break;
        case 0x24: op_LBU(inst.regT(), inst.regS(), inst.imm_se()); break;
        case 0x25: op_LHU(inst.regT(), inst.regS(), inst.imm_se()); break;

        case 0x28: op_SB(inst.regT(), inst.regS(), inst.imm_se()); break;
        case 0x29: op_SH(inst.regT(), inst.regS(), inst.imm_se()); break;

        case 0x2B: op_SW(inst.regT(), inst.regS(), inst.imm_se()); break;
        default:
            assert(false && "Unhandled opcode!");
        }

        memcpy(m_cpuStatus.GPR, m_helperCPURegs, CPU_GPR_COUNT * sizeof(u32));

        assert(getReg(RegIndex{ 0 }) == 0 && "GPR zero value was changed!");
    }

    void CPU::overrideCPURegister(size_t index, u32 value)
    {
        m_cpuStatus.regs[index] = value;
        if (index < CPU_GPR_COUNT)
            m_helperCPURegs[index] = value;
    }

    CPU::CPU()
    {
        memset(m_cpuStatus.regs, 0, CPU_GPR_COUNT * sizeof(u32));
        memset(m_helperCPURegs, 0, CPU_GPR_COUNT * sizeof(u32));
    }

    void CPU::setReg(RegIndex index, u32 value)
    {
        assert(index.i < CPU_GPR_COUNT && "Index out of bounds!");

        m_helperCPURegs[index.i] = value;
        m_helperCPURegs[0] = 0;
    }

    u32 CPU::getReg(RegIndex index) const
    {
        assert(index.i < CPU_GPR_COUNT && "Index out of bounds!");

        return m_cpuStatus.regs[index.i];
    }

    void CPU::exception(Exception cause)
    {
        u32 handler = m_cop0Status.SR.BEV ? 0xBFC00180 : 0x80000080;
        m_cop0Status.SR.IEKUStack <<= 2;
        m_cop0Status.CAUSE = static_cast<u32>(cause) << 2;
        m_cop0Status.EPC = m_currentPC;

        if (m_isBranchDelaySlot) {
            m_cop0Status.EPC -= 4;
            m_cop0Status.CAUSE |= 1 << 31; // set BD flag
        }

        m_cpuStatus.PC = handler;
        m_nextPC = m_cpuStatus.PC + 4;
    }

    void CPU::branch(bool condition, u32 offset)
    {
        if (condition) {
            m_nextPC += offset;
            m_nextPC -= 4;

            m_isBranch = true;
        }
    }

    void CPU::op_MFC0(RegIndex copIndex, RegIndex cpuIndex)
    {
        assert(copIndex.i < COP0_REGISTER_COUNT && "Index out of bounds!");

        m_pendingLoad = { cpuIndex, m_cop0Status.regs[copIndex.i] };
    }

    void CPU::op_MTC0(RegIndex copIndex, RegIndex cpuIndex)
    {
        assert(copIndex.i < COP0_REGISTER_COUNT && "Index out of bounds!");

        m_cop0Status.regs[copIndex.i] = getReg(cpuIndex);
    }

    void CPU::op_RFE()
    {
        m_cop0Status.SR.IEKUStack >>= 2;
    }

    void CPU::op_SYSCALL()
    {
        exception(Exception::SysCall);
    }

    void CPU::op_BXX(RegIndex s, u32 offset, u32 opcode)
    {
        u32 isBGEZ = (opcode >> 16) & 1;
        bool isLink = (opcode >> 20) & 1;

        u32 test = static_cast<s32>(getReg(s)) < 0;
        test = test ^ isBGEZ;

        if (isLink) setReg(RegIndex{ 31 }, m_cpuStatus.PC);
        
        branch(test, offset);
    }

    void CPU::op_SLL(RegIndex d, RegIndex t, u32 shift)
    {
        setReg(d, getReg(t) << shift);
    }

    void CPU::op_SRL(RegIndex d, RegIndex t, u32 shift)
    {
        setReg(d, getReg(t) >> shift);
    }

    void CPU::op_SRA(RegIndex d, RegIndex t, u32 shift)
    {
        setReg(d, static_cast<s32>(getReg(t)) >> shift);
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
        m_nextPC = (m_cpuStatus.PC & 0xF0000000) | immediate;
        m_isBranch = true;
    }

    void CPU::op_JAL(u32 immediate)
    {
        setReg(RegIndex{ 31 }, m_nextPC);
        op_J(immediate);
    }

    void CPU::op_JR(RegIndex s)
    {
        m_nextPC = getReg(s);
        m_isBranch = true;
    }

    void CPU::op_JALR(RegIndex d, RegIndex s)
    {
        setReg(d, m_nextPC);
        m_nextPC = getReg(s);
        m_isBranch = true;
    }

    void CPU::op_ADD(RegIndex d, RegIndex s, u32 rhs)
    {
        // TODO: check this code for egde cases
        s32 a = getReg(s);
        s32 b = to_s32(rhs);
        s32 result = a + b;
        if ((a > 0 && b > 0 && result < 0) || (a < 0 && b < 0 && result > 0))
            exception(Exception::Overflow);
        else
            setReg(d, result);
    }

    void CPU::op_ADDU(RegIndex d, RegIndex s, u32 rhs)
    {
        setReg(d, getReg(s) + rhs);
    }

    void CPU::op_SUBU(RegIndex d, RegIndex s, u32 rhs)
    {
        setReg(d, getReg(s) - rhs);
    }

    void CPU::op_LUI(RegIndex t, u32 immediate)
    {
        setReg(t, immediate << 16);
    }

    void CPU::op_SB(RegIndex t, RegIndex s, u32 immediate)
    {
        if (m_cop0Status.SR.Isc)
            return;

        store8(getReg(s) + immediate, getReg(t) & 0xFF);
    }

    void CPU::op_SH(RegIndex t, RegIndex s, u32 immediate)
    {
        if (m_cop0Status.SR.Isc)
            return;

        u32 address = getReg(s) + immediate;
        if (address % 2 == 0)
            store16(address, getReg(t) & 0xFFFF);
        else
            exception(Exception::StoreAddressError);
    }

    void CPU::op_SW(RegIndex t, RegIndex s, u32 immediate)
    {
        if (m_cop0Status.SR.Isc)
            return;

        u32 address = getReg(s) + immediate;
        if (address % 4 == 0)
            store32(address, getReg(t));
        else
            exception(Exception::StoreAddressError);
    }

    void CPU::op_LB(RegIndex t, RegIndex s, u32 immediate)
    {
        if (m_cop0Status.SR.Isc)
            return;

        s8 value = load8(getReg(s) + immediate);
        m_pendingLoad = { t, static_cast<u32>(value) };
    }

    void CPU::op_LBU(RegIndex t, RegIndex s, u32 immediate)
    {
        if (m_cop0Status.SR.Isc)
            return;

        m_pendingLoad = { t, load8(getReg(s) + immediate) };
    }

    void CPU::op_LHU(RegIndex t, RegIndex s, u32 immediate)
    {
        if (m_cop0Status.SR.Isc)
            return;

        u32 address = getReg(s) + immediate;
        if (address % 2 == 0)
            m_pendingLoad = { t, load16(address) };
        else
            exception(Exception::LoadAddressError);
    }

    void CPU::op_LW(RegIndex t, RegIndex s, u32 immediate)
    {
        if (m_cop0Status.SR.Isc)
            return;

        u32 address = getReg(s) + immediate;
        if (address % 4 == 0)
            m_pendingLoad = { t, load32(address) };
        else
            exception(Exception::LoadAddressError);
    }

    void CPU::op_SLTU(RegIndex d, u32 lhs, u32 rhs)
    {
        setReg(d, lhs < rhs);
    }

    void CPU::op_SLT(RegIndex t, s32 lhs, s32 rhs)
    {
        setReg(t, lhs < rhs);
    }

    void CPU::op_DIV(s32 numerator, s32 denominator)
    {
        if (denominator == 0) {
            m_cpuStatus.HI = numerator;
            m_cpuStatus.LO = (numerator >= 0) ? -1 : 1;
        }
        else if (static_cast<u32>(numerator) == 0x80000000 && denominator == -1) {
            m_cpuStatus.HI = 0;
            m_cpuStatus.LO = 0x80000000;
        }
        else {
            m_cpuStatus.HI = numerator % denominator;
            m_cpuStatus.LO = numerator / denominator;
        }
    }

    void CPU::op_DIVU(u32 numerator, u32 denominator)
    {
        if (denominator == 0) {
            m_cpuStatus.HI = numerator;
            m_cpuStatus.LO = 0xFFFFFFFF;
        }
        else {
            m_cpuStatus.HI = numerator % denominator;
            m_cpuStatus.LO = numerator / denominator;
        }
    }

    void CPU::op_MFHI(RegIndex d)
    {
        setReg(d, m_cpuStatus.HI);
    }

    void CPU::op_MTHI(RegIndex s)
    {
        m_cpuStatus.HI = getReg(s);
    }

    void CPU::op_MFLO(RegIndex d)
    {
        setReg(d, m_cpuStatus.LO);
    }

    void CPU::op_MTLO(RegIndex s)
    {
        m_cpuStatus.LO = getReg(s);
    }

} // namespace PSX
