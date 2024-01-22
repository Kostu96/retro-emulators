#include "cpu.h"
#include "instruction_set.h"

#include <iostream>

void CPU::reset()
{
    m_currentPC = 0xDEADBEEF;
    m_PC = 0xBFC00000; // BIOS start
    m_nextPC = 0xBFC00004;
    m_SR = 0;
    m_CAUSE = 0xDEADBEEF;
    m_EPC = 0xDEADBEEF;
    m_HI = 0xDEADBEEF;
    m_LO = 0xDEADBEEF;
    m_inputRegs.reset();
    m_outputRegs.reset();
    m_pendingLoad = { 0, 0 };
    m_branch = false;
    m_delaySlot = false;
}

void CPU::runNextInstruction()
{
    m_currentPC = m_PC;
    if (m_currentPC % 4 != 0) {
        exception(Expception::LoadAddressError);
        return;
    }

    uint32_t inst = load32(m_PC);
    m_PC = m_nextPC;
    m_nextPC += 4;

    setReg(m_pendingLoad.reg, m_pendingLoad.value);
    m_pendingLoad = { 0, 0 };

    m_delaySlot = m_branch;
    m_branch = false;

    decodeAndExecute(inst);

    // TODO: this is perf bottleneck
    m_inputRegs = m_outputRegs;
}

void CPU::exception(Expception cause)
{
    uint32_t handler = m_SR & (1 << 22) ? 0xBFC00180 : 0x80000080;

    uint32_t mode = m_SR & 0x3F;
    m_SR &= ~0x3F;
    m_SR |= (mode << 2) & 0x3F;

    m_CAUSE = static_cast<uint32_t>(cause) << 2;
    m_EPC = m_currentPC;

    if (m_delaySlot) {
        m_EPC -= 4;
        m_CAUSE |= 1 << 31;
    }

    m_PC = handler;
    m_nextPC = m_PC + 4;
}

void CPU::decodeAndExecute(Instruction instruction)
{
    uint32_t opcode = instruction.opcode();
    InstructionSet::Fns[opcode](*this, instruction);
}
