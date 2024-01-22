#include "instruction_set.h"
#include "cpu.h"

#include <iostream>

void InstructionSet::invalid(CPU& cpu, Instruction instruction)
{
    std::cerr << "Unhandled instruction: " << std::hex << instruction.opcode() << std::dec << '\n';
    cpu.exception(CPU::Expception::IllegallInstruction);
}

void InstructionSet::zero(CPU& cpu, Instruction instruction)
{
    // TODO: change this to array:
    uint32_t subfunction = instruction.subfn();
    switch (subfunction) {
    case 0x00: SLL(cpu, instruction);     break;
    case 0x02: SRL(cpu, instruction);     break;
    case 0x03: SRA(cpu, instruction);     break;
    case 0x04: SLLV(cpu, instruction);    break;
    case 0x06: SRLV(cpu, instruction);    break;
    case 0x07: SRAV(cpu, instruction);    break;
    case 0x08: JR(cpu, instruction);      break;
    case 0x09: JALR(cpu, instruction);    break;
    case 0x0C: SYSCALL(cpu, instruction); break;
    case 0x0D: BREAK(cpu, instruction);   break;
    case 0x10: MFHI(cpu, instruction);    break;
    case 0x11: MTHI(cpu, instruction);    break;
    case 0x12: MFLO(cpu, instruction);    break;
    case 0x13: MTLO(cpu, instruction);    break;
    case 0x1A: DIV(cpu, instruction);     break;
    case 0x1B: DIVU(cpu, instruction);    break;
    case 0x18: MULT(cpu, instruction);    break;
    case 0x19: MULTU(cpu, instruction);   break;
    case 0x20: ADD(cpu, instruction);     break;
    case 0x21: ADDU(cpu, instruction);    break;
    case 0x22: SUB(cpu, instruction);     break;
    case 0x23: SUBU(cpu, instruction);    break;
    case 0x24: AND(cpu, instruction);     break;
    case 0x25: OR(cpu, instruction);      break;
    case 0x26: XOR(cpu, instruction);     break;
    case 0x27: NOR(cpu, instruction);     break;
    case 0x2A: SLT(cpu, instruction);     break;
    case 0x2B: SLTU(cpu, instruction);    break;
    default:
        std::cerr << "Unhandled zero instruction: " << std::hex << instruction.subfn() << std::dec << '\n';
    }
}

void InstructionSet::bxxxxx(CPU& cpu, Instruction instruction)
{
    uint32_t imm = instruction.imm_se();
    RegisterIndex s = instruction.regS();

    uint32_t isBGEZ = (instruction.word >> 16) & 1;
    bool isLink = (instruction.word >> 20) & 1;

    int32_t value = cpu.getReg(s);
    uint32_t test = value < 0;
    test = test ^ isBGEZ;

    if (test != 0) {
        if (isLink)
            cpu.setReg(31, cpu.m_PC);

        cpu.m_nextPC += (imm << 2);
        cpu.m_nextPC -= 4;
    }
    cpu.m_branch = true;
}

void InstructionSet::SLL(CPU& cpu, Instruction instruction)
{
    RegisterIndex source = instruction.regT();
    RegisterIndex target = instruction.regD();
    uint32_t value = cpu.getReg(source);
    value <<= instruction.shift();
    cpu.setReg(target, value);
}

void InstructionSet::SRL(CPU& cpu, Instruction instruction)
{
    RegisterIndex source = instruction.regT();
    RegisterIndex target = instruction.regD();
    uint32_t value = cpu.getReg(source);
    value >>= instruction.shift();
    cpu.setReg(target, value);
}

void InstructionSet::SRA(CPU& cpu, Instruction instruction)
{
    RegisterIndex t = instruction.regT();
    RegisterIndex d = instruction.regD();
    int32_t value = cpu.getReg(t);
    value >>= instruction.shift();
    cpu.setReg(d, value);
}

void InstructionSet::SLLV(CPU& cpu, Instruction instruction)
{
    RegisterIndex d = instruction.regD();
    RegisterIndex s = instruction.regS();
    RegisterIndex t = instruction.regT();
    uint32_t value = cpu.getReg(t) << (cpu.getReg(s) & 0x1F);
    cpu.setReg(d, value);
}

void InstructionSet::SRLV(CPU& cpu, Instruction instruction)
{
    RegisterIndex d = instruction.regD();
    RegisterIndex s = instruction.regS();
    RegisterIndex t = instruction.regT();
    uint32_t value = cpu.getReg(t) >> (cpu.getReg(s) & 0x1F);
    cpu.setReg(d, value);
}

void InstructionSet::SRAV(CPU& cpu, Instruction instruction)
{
    RegisterIndex d = instruction.regD();
    uint32_t s = cpu.getReg(instruction.regS());
    int32_t t = cpu.getReg(instruction.regT());
    uint32_t value = t >> (s & 0x1F);
    cpu.setReg(d, value);
}

void InstructionSet::JR(CPU& cpu, Instruction instruction)
{
    RegisterIndex s = instruction.regS();
    cpu.m_nextPC = cpu.getReg(s);
    cpu.m_branch = true;
}

void InstructionSet::JALR(CPU& cpu, Instruction instruction)
{
    RegisterIndex d = instruction.regD();
    RegisterIndex s = instruction.regS();
    cpu.setReg(d, cpu.m_nextPC);
    cpu.m_nextPC = cpu.getReg(s);
    cpu.m_branch = true;
}

void InstructionSet::SYSCALL(CPU& cpu, Instruction /*instruction*/)
{
    cpu.exception(CPU::Expception::SysCall);
}

void InstructionSet::BREAK(CPU& cpu, Instruction /*instruction*/)
{
    cpu.exception(CPU::Expception::Break);
}

void InstructionSet::MFHI(CPU& cpu, Instruction instruction)
{
    RegisterIndex d = instruction.regD();
    cpu.setReg(d, cpu.m_HI);
}

void InstructionSet::MTHI(CPU& cpu, Instruction instruction)
{
    RegisterIndex s = instruction.regS();
    cpu.m_HI = cpu.getReg(s);
}

void InstructionSet::MFLO(CPU& cpu, Instruction instruction)
{
    RegisterIndex d = instruction.regD();
    cpu.setReg(d, cpu.m_LO);
}

void InstructionSet::MTLO(CPU& cpu, Instruction instruction)
{
    RegisterIndex s = instruction.regS();
    cpu.m_LO = cpu.getReg(s);
}

void InstructionSet::DIV(CPU& cpu, Instruction instruction)
{
    RegisterIndex s = instruction.regS();
    RegisterIndex t = instruction.regT();
    int32_t n = cpu.getReg(s);
    int32_t d = cpu.getReg(t);
    if (d == 0) {
        // division by 0
        cpu.m_HI = n;
        cpu.m_LO = (n >= 0) ? 0xFFFFFFFF : 1;
    }
    else if (static_cast<uint32_t>(n) == 0x80000000 && d == -1) {
        cpu.m_HI = 0;
        cpu.m_LO = 0x80000000;
    }
    else {
        cpu.m_HI = n % d;
        cpu.m_LO = n / d;
    }
}

void InstructionSet::DIVU(CPU& cpu, Instruction instruction)
{
    RegisterIndex s = instruction.regS();
    RegisterIndex t = instruction.regT();
    uint32_t n = cpu.getReg(s);
    uint32_t d = cpu.getReg(t);
    if (d == 0) {
        // division by 0
        cpu.m_HI = n;
        cpu.m_LO = 0xFFFFFFFF;
    }
    else {
        cpu.m_HI = n % d;
        cpu.m_LO = n / d;
    }
}

void InstructionSet::MULT(CPU& cpu, Instruction instruction)
{
    int64_t s = static_cast<int32_t>(cpu.getReg(instruction.regS()));
    int64_t t = static_cast<int32_t>(cpu.getReg(instruction.regT()));
    uint64_t value = s * t;
    cpu.m_HI = value >> 32;
    cpu.m_LO = value & 0xFFFFFFFF;
}

void InstructionSet::MULTU(CPU& cpu, Instruction instruction)
{
    uint64_t s = cpu.getReg(instruction.regS());
    uint64_t t = cpu.getReg(instruction.regT());
    uint64_t value = s * t;
    cpu.m_HI = value >> 32;
    cpu.m_LO = value & 0xFFFFFFFF;
}

void InstructionSet::ADD(CPU& cpu, Instruction instruction)
{
    RegisterIndex s = instruction.regS();
    RegisterIndex t = instruction.regT();
    RegisterIndex d = instruction.regD();
    int32_t a = cpu.getReg(s);
    int32_t b = cpu.getReg(t);
    uint32_t value = a + b;
    if (a > 0 && b > 0 && value < 0 ||
        a < 0 && b < 0 && value > 0)
        cpu.exception(CPU::Expception::Overflow);
    else
        cpu.setReg(d, value);
}

void InstructionSet::ADDU(CPU& cpu, Instruction instruction)
{
    RegisterIndex s = instruction.regS();
    RegisterIndex t = instruction.regT();
    RegisterIndex d = instruction.regD();
    uint32_t value = cpu.getReg(s) + cpu.getReg(t);
    cpu.setReg(d, value);
}

void InstructionSet::SUB(CPU& cpu, Instruction instruction)
{
    int32_t s = cpu.getReg(instruction.regS());
    int32_t t = cpu.getReg(instruction.regT());
    RegisterIndex d = instruction.regD();
    int32_t value = s - t;
    if (s > 0 && t < 0 && value < 0 ||
        s < 0 && t > 0 && value > 0)
        cpu.exception(CPU::Expception::Overflow);
    else
        cpu.setReg(d, value);
}

void InstructionSet::SUBU(CPU& cpu, Instruction instruction)
{
    RegisterIndex s = instruction.regS();
    RegisterIndex t = instruction.regT();
    RegisterIndex d = instruction.regD();

    uint32_t value = cpu.getReg(s) - cpu.getReg(t);
    cpu.setReg(d, value);
}

void InstructionSet::AND(CPU& cpu, Instruction instruction)
{
    RegisterIndex d = instruction.regD();
    RegisterIndex s = instruction.regS();
    RegisterIndex t = instruction.regT();
    uint32_t value = cpu.getReg(s) & cpu.getReg(t);
    cpu.setReg(d, value);
}

void InstructionSet::OR(CPU& cpu, Instruction instruction)
{
    RegisterIndex d = instruction.regD();
    RegisterIndex s = instruction.regS();
    RegisterIndex t = instruction.regT();
    uint32_t value = cpu.getReg(s) | cpu.getReg(t);
    cpu.setReg(d, value);
}

void InstructionSet::XOR(CPU& cpu, Instruction instruction)
{
    RegisterIndex d = instruction.regD();
    RegisterIndex s = instruction.regS();
    RegisterIndex t = instruction.regT();
    uint32_t value = cpu.getReg(s) ^ cpu.getReg(t);
    cpu.setReg(d, value);
}

void InstructionSet::NOR(CPU& cpu, Instruction instruction)
{
    RegisterIndex d = instruction.regD();
    RegisterIndex s = instruction.regS();
    RegisterIndex t = instruction.regT();
    uint32_t value = ~(cpu.getReg(s) | cpu.getReg(t));
    cpu.setReg(d, value);
}

void InstructionSet::SLT(CPU& cpu, Instruction instruction)
{
    RegisterIndex d = instruction.regD();
    RegisterIndex s = instruction.regS();
    RegisterIndex t = instruction.regT();
    uint32_t value = static_cast<int32_t>(cpu.getReg(s)) < static_cast<int32_t>(cpu.getReg(t));
    cpu.setReg(d, value);
}

void InstructionSet::SLTU(CPU& cpu, Instruction instruction)
{
    RegisterIndex d = instruction.regD();
    RegisterIndex s = instruction.regS();
    RegisterIndex t = instruction.regT();
    uint32_t value = cpu.getReg(s) < cpu.getReg(t);
    cpu.setReg(d, value);
}

void InstructionSet::J(CPU& cpu, Instruction instruction)
{
    cpu.m_nextPC = (cpu.m_nextPC & 0xF0000000) | (instruction.imm_jump() << 2); // TODO: move this shift to imm_jump()?
    cpu.m_branch = true;
}

void InstructionSet::JAL(CPU& cpu, Instruction instruction)
{
    cpu.setReg(31, cpu.m_nextPC);
    J(cpu, instruction);
    // cpu.m_branch = true; - redundant
}

void InstructionSet::BEQ(CPU& cpu, Instruction instruction)
{
    uint32_t imm = instruction.imm_se();
    RegisterIndex s = instruction.regS();
    RegisterIndex t = instruction.regT();

    if (cpu.getReg(s) == cpu.getReg(t)) {
        cpu.m_nextPC += (imm << 2);
        cpu.m_nextPC -= 4;
    }
    cpu.m_branch = true;
}

void InstructionSet::BNE(CPU& cpu, Instruction instruction)
{
    uint32_t imm = instruction.imm_se();
    RegisterIndex s = instruction.regS();
    RegisterIndex t = instruction.regT();

    if (cpu.getReg(s) != cpu.getReg(t)) {
        cpu.m_nextPC += (imm << 2);
        cpu.m_nextPC -= 4;
    }
    cpu.m_branch = true;
}

void InstructionSet::BLEZ(CPU& cpu, Instruction instruction)
{
    uint32_t imm = instruction.imm_se();
    RegisterIndex s = instruction.regS();
    int32_t value = cpu.getReg(s);

    if (value <= 0) {
        cpu.m_nextPC += (imm << 2);
        cpu.m_nextPC -= 4;
    }
    cpu.m_branch = true;
}

void InstructionSet::BGTZ(CPU& cpu, Instruction instruction)
{
    uint32_t imm = instruction.imm_se();
    RegisterIndex s = instruction.regS();
    int32_t value = cpu.getReg(s);

    if (value  > 0) {
        cpu.m_nextPC += (imm << 2);
        cpu.m_nextPC -= 4;
    }
    cpu.m_branch = true;
}

void InstructionSet::ADDI(CPU& cpu, Instruction instruction)
{
    RegisterIndex t = instruction.regT();
    RegisterIndex s = instruction.regS();
    int32_t a = cpu.getReg(s);
    int32_t b = instruction.imm_se();
    uint32_t value = a + b;
    if (a > 0 && b > 0 && value < 0 ||
        a < 0 && b < 0 && value > 0)
        cpu.exception(CPU::Expception::Overflow);
    else
        cpu.setReg(t, value);
}

void InstructionSet::ADDIU(CPU& cpu, Instruction instruction)
{
    RegisterIndex target = instruction.regT();
    RegisterIndex source = instruction.regS();
    uint32_t value = cpu.getReg(source) + instruction.imm_se();
    cpu.setReg(target, value);
}

void InstructionSet::SLTI(CPU& cpu, Instruction instruction)
{
    int32_t imm = instruction.imm_se();
    RegisterIndex s = instruction.regS();
    RegisterIndex t = instruction.regT();

    uint32_t value = static_cast<int32_t>(cpu.getReg(s)) < imm;
    cpu.setReg(t, value);
}

void InstructionSet::SLTIU(CPU& cpu, Instruction instruction)
{
    uint32_t imm = instruction.imm_se();
    RegisterIndex s = instruction.regS();
    RegisterIndex t = instruction.regT();

    uint32_t value = cpu.getReg(s) < imm;
    cpu.setReg(t, value);
}

void InstructionSet::ANDI(CPU& cpu, Instruction instruction)
{
    RegisterIndex t = instruction.regT();
    RegisterIndex s = instruction.regS();
    uint32_t value = cpu.getReg(s) & instruction.imm();
    cpu.setReg(t, value);
}

void InstructionSet::ORI(CPU& cpu, Instruction instruction)
{
    RegisterIndex t = instruction.regT();
    RegisterIndex s = instruction.regS();
    uint32_t value = cpu.getReg(s) | instruction.imm();
    cpu.setReg(t, value);
}

void InstructionSet::XORI(CPU& cpu, Instruction instruction)
{
    RegisterIndex t = instruction.regT();
    RegisterIndex s = instruction.regS();
    uint32_t value = cpu.getReg(s) ^ instruction.imm();
    cpu.setReg(t, value);
}

void InstructionSet::LUI(CPU& cpu, Instruction instruction)
{
    uint32_t imm = instruction.imm();
    RegisterIndex target = instruction.regT();
    uint32_t value = imm << 16;
	cpu.setReg(target, value);
}

void InstructionSet::cop0(CPU& cpu, Instruction instruction)
{
    uint32_t copfn = instruction.copfn();
    switch (copfn) {
    case 0x00: MFC0(cpu, instruction); break;
    case 0x04: MTC0(cpu, instruction); break;
    case 0x10: RFE(cpu, instruction);  break;
    default:
        std::cerr << "Unhandled cop0 instruction: " << std::hex << instruction.copfn() << std::dec << '\n';
    }
}

void InstructionSet::cop1(CPU& cpu, Instruction /*instruction*/)
{
    cpu.exception(CPU::Expception::CoprocessorError);
}

void InstructionSet::cop2(CPU& cpu, Instruction instruction)
{
    std::cerr << "Unhandled cop2!";
    abort();
}

void InstructionSet::cop3(CPU& cpu, Instruction /*instruction*/)
{
    cpu.exception(CPU::Expception::CoprocessorError);
}

void InstructionSet::MFC0(CPU& cpu, Instruction instruction)
{
    RegisterIndex cpuReg = instruction.regT();
    RegisterIndex copReg = instruction.regD();
    uint32_t value = 0xDEADBEEF;
    
    switch (copReg.index) {
    case 12:
        value = cpu.m_SR;
        break;
    case 13:
        value = cpu.m_CAUSE;
        break;
    case 14:
        value = cpu.m_EPC;
        break;
    default:
        std::cerr << "Unhandled MFC0 instruction\n";
    }

    cpu.m_pendingLoad = { cpuReg, value };
}

void InstructionSet::MTC0(CPU& cpu, Instruction instruction)
{
    // TODO: temp
    RegisterIndex cpuReg = instruction.regT();
    RegisterIndex copReg = instruction.regD();
    uint32_t value = cpu.getReg(cpuReg);

    switch (copReg.index) {
    case 3:
    case 5:
    case 6:
    case 7:
    case 9:
    case 11:
        if (value != 0)
            std::cerr << "Unhandled write to breakpoints registers\n";
        break; // ignore these if 0
    case 12:
        cpu.m_SR = value;
        break;
    case 13:
        if (value != 0)
            std::cerr << "Unhandled write to CAUSE register\n";
        break; // CAUSE register - ignore if 0
    default:
        std::cerr << "Unhandled MTC0 instruction\n";
    }
}

void InstructionSet::RFE(CPU& cpu, Instruction instruction)
{
    if ((instruction.word & 0x3F) != 0x10) {
        std::cerr << "Invalid COP0 instruction: " << instruction.word << '\n';
        abort();
    }

    uint32_t mode = cpu.m_SR & 0x3F;
    cpu.m_SR &= ~0x3F;
    cpu.m_SR |= mode >> 2;
}

void InstructionSet::LB(CPU& cpu, Instruction instruction)
{
    if (cpu.m_SR & 0x10000)
        return; // isolated cache bit is set

    uint32_t imm = instruction.imm_se();
    RegisterIndex t = instruction.regT();
    RegisterIndex s = instruction.regS();
    uint32_t address = cpu.getReg(s) + imm;
    uint32_t value = static_cast<int8_t>(cpu.load8(address));
    cpu.m_pendingLoad = { t, value };
}

void InstructionSet::LH(CPU& cpu, Instruction instruction)
{
    if (cpu.m_SR & 0x10000)
        return; // isolated cache bit is set

    uint32_t imm = instruction.imm_se();
    RegisterIndex t = instruction.regT();
    RegisterIndex s = instruction.regS();
    uint32_t address = cpu.getReg(s) + imm;
    uint32_t value = static_cast<int16_t>(cpu.load16(address));

    if (address % 2 == 0)
        cpu.m_pendingLoad = { t, value };
    else
        cpu.exception(CPU::Expception::LoadAddressError);
}

void InstructionSet::LWL(CPU& cpu, Instruction instruction)
{
    uint32_t imm = instruction.imm_se();
    RegisterIndex t = instruction.regT();
    uint32_t current = cpu.m_outputRegs.get(t);
    uint32_t s = cpu.getReg(instruction.regS());
    uint32_t address = s + imm;
    uint32_t alignedWord = cpu.load32(address & ~0x3);
    uint32_t value = 0xDEADBEEF;
    switch (address & 0x3) {
    case 0: value = (current & 0x00FFFFFF) | (alignedWord << 24); break;
    case 1: value = (current & 0x0000FFFF) | (alignedWord << 16); break;
    case 2: value = (current & 0x000000FF) | (alignedWord << 8); break;
    case 3: value = (current & 0x00000000) | alignedWord; break;
    }
    cpu.m_pendingLoad = { t, value };
}

void InstructionSet::LW(CPU& cpu, Instruction instruction)
{
    if (cpu.m_SR & 0x10000)
        return; // isolated cache bit is set

    uint32_t imm = instruction.imm_se();
    RegisterIndex t = instruction.regT();
    RegisterIndex s = instruction.regS();
    uint32_t address = cpu.getReg(s) + imm;
    uint32_t value = cpu.load32(address);

    if (address % 4 == 0)
        cpu.m_pendingLoad = { t, value };
    else
        cpu.exception(CPU::Expception::LoadAddressError);
}

void InstructionSet::LBU(CPU& cpu, Instruction instruction)
{
    if (cpu.m_SR & 0x10000)
        return; // isolated cache bit is set

    uint32_t imm = instruction.imm_se();
    RegisterIndex t = instruction.regT();
    RegisterIndex s = instruction.regS();
    uint32_t address = cpu.getReg(s) + imm;
    uint32_t value = cpu.load8(address);
    cpu.m_pendingLoad = { t, value };
}

void InstructionSet::LHU(CPU& cpu, Instruction instruction)
{
    if (cpu.m_SR & 0x10000)
        return; // isolated cache bit is set

    uint32_t imm = instruction.imm_se();
    RegisterIndex t = instruction.regT();
    RegisterIndex s = instruction.regS();
    uint32_t address = cpu.getReg(s) + imm;
    uint32_t value = cpu.load16(address);
    
    if (address % 2 == 0)
        cpu.m_pendingLoad = { t, value };
    else
        cpu.exception(CPU::Expception::LoadAddressError);
}

void InstructionSet::LWR(CPU& cpu, Instruction instruction)
{
    uint32_t imm = instruction.imm_se();
    RegisterIndex t = instruction.regT();
    uint32_t current = cpu.m_outputRegs.get(t);
    uint32_t s = cpu.getReg(instruction.regS());
    uint32_t address = s + imm;
    uint32_t alignedWord = cpu.load32(address & ~0x3);
    uint32_t value = 0xDEADBEEF;
    switch (address & 0x3) {
    case 0: value = (current & 0x00000000) | alignedWord; break;
    case 1: value = (current & 0xFF000000) | (alignedWord >> 8); break;
    case 2: value = (current & 0xFFFF0000) | (alignedWord >> 16); break;
    case 3: value = (current & 0xFFFFFF00) | (alignedWord >> 24); break;
    }
    cpu.m_pendingLoad = { t, value };
}

void InstructionSet::SB(CPU& cpu, Instruction instruction)
{
    if (cpu.m_SR & 0x10000)
        return; // isolated cache bit is set

    uint32_t imm = instruction.imm_se();
    RegisterIndex t = instruction.regT();
    RegisterIndex s = instruction.regS();
    uint32_t address = cpu.getReg(s) + imm;
    uint8_t value = cpu.getReg(t);
    cpu.store8(address, value);
}

void InstructionSet::SH(CPU& cpu, Instruction instruction)
{
    if (cpu.m_SR & 0x10000)
        return; // isolated cache bit is set

    uint32_t imm = instruction.imm_se();
    RegisterIndex t = instruction.regT();
    RegisterIndex s = instruction.regS();
    uint32_t address = cpu.getReg(s) + imm;
    uint16_t value = cpu.getReg(t);

    if (address % 2 == 0)
        cpu.store16(address, value);
    else
        cpu.exception(CPU::Expception::StoreAddressError);
}

void InstructionSet::SWL(CPU& cpu, Instruction instruction)
{
    if (cpu.m_SR & 0x10000)
        return; // isolated cache bit is set

    uint32_t imm = instruction.imm_se();
    uint32_t t = cpu.getReg(instruction.regT());
    uint32_t s = cpu.getReg(instruction.regS());
    uint32_t address = s + imm;
    uint32_t current = cpu.load32(address & ~0x3);
    uint32_t value = 0xDEADBEEF;
    switch (address & 0x3) {
    case 0: value = (current & 0x00FFFFFF) | (t >> 24); break;
    case 1: value = (current & 0x0000FFFF) | (t >> 16); break;
    case 2: value = (current & 0x000000FF) | (t >> 8); break;
    case 3: value = (current & 0x00000000) | t; break;
    }
    cpu.store32(address & ~0x3, value);
}

void InstructionSet::SW(CPU& cpu, Instruction instruction)
{
    if (cpu.m_SR & 0x10000)
        return; // isolated cache bit is set

    uint32_t imm = instruction.imm_se();
    RegisterIndex t = instruction.regT();
    RegisterIndex s = instruction.regS();
    uint32_t address = cpu.getReg(s) + imm;
    uint32_t value = cpu.getReg(t);
    
    if (address % 4 == 0)
        cpu.store32(address, value);
    else
        cpu.exception(CPU::Expception::StoreAddressError);
}

void InstructionSet::SWR(CPU& cpu, Instruction instruction)
{
    if (cpu.m_SR & 0x10000)
        return; // isolated cache bit is set

    uint32_t imm = instruction.imm_se();
    uint32_t t = cpu.getReg(instruction.regT());
    uint32_t s = cpu.getReg(instruction.regS());
    uint32_t address = s + imm;
    uint32_t current = cpu.load32(address & ~0x3);
    uint32_t value = 0xDEADBEEF;
    switch (address & 0x3) {
    case 0: value = (current & 0x00000000) | t; break;
    case 1: value = (current & 0xFF000000) | (t << 8); break;
    case 2: value = (current & 0xFFFF0000) | (t << 16); break;
    case 3: value = (current & 0xFFFFFF00) | (t << 24); break;
    }
    cpu.store32(address & ~0x3, value);
}

void InstructionSet::LWC0(CPU& cpu, Instruction /*instruction*/)
{
    cpu.exception(CPU::Expception::CoprocessorError);
}

void InstructionSet::LWC1(CPU& cpu, Instruction /*instruction*/)
{
    cpu.exception(CPU::Expception::CoprocessorError);
}

void InstructionSet::LWC2(CPU& cpu, Instruction instruction)
{
    std::cerr << "Unhandled LWC2\n";
}

void InstructionSet::LWC3(CPU& cpu, Instruction /*instruction*/)
{
    cpu.exception(CPU::Expception::CoprocessorError);
}

void InstructionSet::SWC0(CPU& cpu, Instruction /*instruction*/)
{
    cpu.exception(CPU::Expception::CoprocessorError);
}

void InstructionSet::SWC1(CPU& cpu, Instruction /*instruction*/)
{
    cpu.exception(CPU::Expception::CoprocessorError);
}

void InstructionSet::SWC2(CPU& cpu, Instruction instruction)
{
    std::cerr << "Unhandled SWC2\n";
}

void InstructionSet::SWC3(CPU& cpu, Instruction /*instruction*/)
{
    cpu.exception(CPU::Expception::CoprocessorError);
}

InstructionSet::InstFuncType InstructionSet::Fns[] = {
    zero,    // 0x00
    bxxxxx,  // 0x01
    J,       // 0x02
    JAL,     // 0x03
    BEQ,     // 0x04
    BNE,     // 0x05
    BLEZ,    // 0x06
    BGTZ,    // 0x07
    ADDI,    // 0x08
    ADDIU,   // 0x09
    SLTI,    // 0x0A
    SLTIU,   // 0x0B
    ANDI,    // 0x0C
    ORI,     // 0x0D
    XORI,    // 0x0E
    LUI,     // 0x0F
    cop0,    // 0x10
    cop1,    // 0x11
    cop2,    // 0x12
    cop3,    // 0x13
    invalid, // 0x14
    invalid, // 0x15
    invalid, // 0x16
    invalid, // 0x17
    invalid, // 0x18
    invalid, // 0x19
    invalid, // 0x1A
    invalid, // 0x1B
    invalid, // 0x1C
    invalid, // 0x1D
    invalid, // 0x1E
    invalid, // 0x1F
    LB,      // 0x20
    LH,      // 0x21
    LWL,     // 0x22
    LW,      // 0x23
    LBU,     // 0x24
    LHU,     // 0x25
    LWR,     // 0x26
    invalid, // 0x27
    SB,      // 0x28
    SH,      // 0x29
    SWL,     // 0x2A
    SW,      // 0x2B
    invalid, // 0x2C
    invalid, // 0x2D
    SWR,     // 0x2E
    invalid, // 0x2F
    LWC0,    // 0x30
    LWC1,    // 0x31
    LWC2,    // 0x32
    LWC3,    // 0x33
    invalid, // 0x34
    invalid, // 0x35
    invalid, // 0x36
    invalid, // 0x37
    SWC0,    // 0x38
    SWC1,    // 0x39
    SWC2,    // 0x3A
    SWC3,    // 0x3B
    invalid, // 0x3C
    invalid, // 0x3D
    invalid, // 0x3E
    invalid  // 0x3F
};
