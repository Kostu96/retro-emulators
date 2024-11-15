#include "disasm.hpp"

#include <cassert>

namespace PSX {

    void disasm(u32 address, CPUInstruction inst, DisassemblyLine& output)
    {
        output.address = address; // TODO(Kostu): this is not a good solution when memory address is translated

        switch (inst.opcode())
        {
        case 0x00:
            switch (inst.subfn())
            {
            case 0x00: {
                if (inst.word == 0) sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "NOP");
                else sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SLL    $%u, $%u, %u", inst.regD().i, inst.regT().i, inst.shift());
            } break;

            case 0x02: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SRL    $%u, $%u, %u", inst.regD().i, inst.regT().i, inst.shift()); break;
            case 0x03: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SRA    $%u, $%u, %u", inst.regD().i, inst.regT().i, inst.shift()); break;
            case 0x04: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SLLV   $%u, $%u, $%u", inst.regD().i, inst.regT().i, inst.regS().i); break;
            
            case 0x06: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SRLV   $%u, $%u, $%u", inst.regD().i, inst.regT().i, inst.regS().i); break;
            case 0x07: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SRAV   $%u, $%u, $%u", inst.regD().i, inst.regT().i, inst.regS().i); break;
            case 0x08: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "JR     $%u", inst.regS().i); break;
            case 0x09: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "JALR   $%u, $%u", inst.regD().i, inst.regS().i); break;

            case 0x0C: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SYSCALL"); break;
            case 0x0D: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "BREAK"); break;

            case 0x10: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "MFHI   $%u", inst.regD().i); break;
            case 0x11: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "MTHI   $%u", inst.regS().i); break;
            case 0x12: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "MFLO   $%u", inst.regD().i); break;
            case 0x13: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "MTLO   $%u", inst.regS().i); break;

            case 0x1A: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "DIV    $%u, $%u", inst.regS().i, inst.regT().i); break;
            case 0x1B: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "DIVU   $%u, $%u", inst.regS().i, inst.regT().i); break;

            case 0x18: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "MULT   $%u, $%u", inst.regS().i, inst.regT().i); break;
            case 0x19: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "MULTU  $%u, $%u", inst.regS().i, inst.regT().i); break;
            case 0x20: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "ADD    $%u, $%u, $%u", inst.regD().i, inst.regS().i, inst.regT().i); break;
            case 0x21: {
                if (inst.regT().i == 0) sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "MOVE   $%u, $%u", inst.regD().i, inst.regS().i);
                else sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "ADDU   $%u, $%u, $%u", inst.regD().i, inst.regS().i, inst.regT().i);
            } break;
            case 0x22: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SUB    $%u, $%u, $%u", inst.regD().i, inst.regS().i, inst.regT().i); break;
            case 0x23: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SUBU   $%u, $%u, $%u", inst.regD().i, inst.regS().i, inst.regT().i); break;
            case 0x24: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "AND    $%u, $%u, $%u", inst.regD().i, inst.regS().i, inst.regT().i); break;
            case 0x25: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "OR     $%u, $%u, $%u", inst.regD().i, inst.regS().i, inst.regT().i); break;
            case 0x26: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "XOR    $%u, $%u, $%u", inst.regD().i, inst.regS().i, inst.regT().i); break;
            case 0x27: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "NOR    $%u, $%u, $%u", inst.regD().i, inst.regS().i, inst.regT().i); break;

            case 0x2A: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SLT    $%u, $%u, $%u", inst.regD().i, inst.regS().i, inst.regT().i); break;
            case 0x2B: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SLTU   $%u, $%u, $%u", inst.regD().i, inst.regS().i, inst.regT().i); break;
            default: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "??? ZERO ???"); break;
            }
            break;
        case 0x01: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "B%sZ%s $%u, 0x%X", (inst.word >> 16) & 1 ? "GE" : "LT", (inst.word >> 20) & 1 ? "AL" : "  ", inst.regS().i, inst.imm_se_jump()); break;
        case 0x02: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "J      0x%X", inst.imm_jump()); break;
        case 0x03: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "JAL    0x%X", inst.imm_jump()); break;
        case 0x04: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "BEQ    $%u, $%u, %d", inst.regS().i, inst.regT().i, inst.imm_se_jump()); break;
        case 0x05: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "BNE    $%u, $%u, %d", inst.regS().i, inst.regT().i, inst.imm_se_jump()); break;
        case 0x06: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "BLEZ   $%u, %d", inst.regS().i, inst.imm_se_jump()); break;
        case 0x07: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "BGTZ   $%u, %d", inst.regS().i, inst.imm_se_jump()); break;
        case 0x08: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "ADDI   $%u, $%u, 0x%X", inst.regT().i, inst.regS().i, inst.imm()); break;
        case 0x09: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "ADDIU  $%u, $%u, 0x%X", inst.regT().i, inst.regS().i, inst.imm()); break;
        case 0x0A: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SLTI   $%u, $%u, 0x%X", inst.regT().i, inst.regS().i, inst.imm()); break;
        case 0x0B: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SLTIU  $%u, $%u, 0x%X", inst.regT().i, inst.regS().i, inst.imm()); break;
        case 0x0C: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "ANDI   $%u, $%u, 0x%X", inst.regT().i, inst.regS().i, inst.imm()); break;
        case 0x0D: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "ORI    $%u, $%u, 0x%X", inst.regT().i, inst.regS().i, inst.imm()); break;
        case 0x0E: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "XORI   $%u, $%u, 0x%X", inst.regT().i, inst.regS().i, inst.imm()); break;
        case 0x0F: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "LUI    $%u, 0x%X", inst.regT().i, inst.imm()); break;
        case 0x10:
            switch (inst.copfn())
            {
            case 0x00: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "MFC0   $%u, $%u", inst.regD().i, inst.regT().i); break;
            case 0x04: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "MTC0   $%u, $%u", inst.regD().i, inst.regT().i); break;
            case 0x10: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "RFE"); break;
            default: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "??? COP0 ???"); break;
            }
            break;
        case 0x11: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "??? COP1 ???"); break;
        case 0x12: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "??? COP2 ???"); break;
        case 0x13: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "??? COP3 ???"); break;

        case 0x20: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "LB     $%u, 0x%X($%u)", inst.regT().i, inst.imm(), inst.regS().i); break;
        case 0x21: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "LH     $%u, 0x%X($%u)", inst.regT().i, inst.imm(), inst.regS().i); break;
        case 0x22: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "LWL    $%u, 0x%X($%u)", inst.regT().i, inst.imm(), inst.regS().i); break;
        case 0x23: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "LW     $%u, 0x%X($%u)", inst.regT().i, inst.imm(), inst.regS().i); break;
        case 0x24: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "LBU    $%u, 0x%X($%u)", inst.regT().i, inst.imm(), inst.regS().i); break;
        case 0x25: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "LHU    $%u, 0x%X($%u)", inst.regT().i, inst.imm(), inst.regS().i); break;
        case 0x26: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "LWR    $%u, 0x%X($%u)", inst.regT().i, inst.imm(), inst.regS().i); break;
        
        case 0x28: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SB     $%u, 0x%X($%u)", inst.regT().i, inst.imm(), inst.regS().i); break;
        case 0x29: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SH     $%u, 0x%X($%u)", inst.regT().i, inst.imm(), inst.regS().i); break;
        case 0x2A: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SWL    $%u, 0x%X($%u)", inst.regT().i, inst.imm(), inst.regS().i); break;
        case 0x2B: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SW     $%u, 0x%X($%u)", inst.regT().i, inst.imm(), inst.regS().i); break;
        
        case 0x2E: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SWR    $%u, 0x%X($%u)", inst.regT().i, inst.imm(), inst.regS().i); break;
        
        case 0x30: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "LWC0   $%u, 0x%X($%u)", inst.regT().i, inst.imm(), inst.regS().i); break;
        case 0x31: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "LWC1   $%u, 0x%X($%u)", inst.regT().i, inst.imm(), inst.regS().i); break;
        case 0x32: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "LWC2   $%u, 0x%X($%u)", inst.regT().i, inst.imm(), inst.regS().i); break;
        case 0x33: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "LWC3   $%u, 0x%X($%u)", inst.regT().i, inst.imm(), inst.regS().i); break;
        
        case 0x38: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SWC0   $%u, 0x%X($%u)", inst.regT().i, inst.imm(), inst.regS().i); break;
        case 0x39: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SWC1   $%u, 0x%X($%u)", inst.regT().i, inst.imm(), inst.regS().i); break;
        case 0x3A: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SWC2   $%u, 0x%X($%u)", inst.regT().i, inst.imm(), inst.regS().i); break;
        case 0x3B: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SWC3   $%u, 0x%X($%u)", inst.regT().i, inst.imm(), inst.regS().i); break;
        default: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "??? ??? ???"); break;
        }
    }

} // namespace PSX
