#include "disasm.hpp"

#include <cassert>

namespace PSX {

    void disasm(u32 address, CPU::Instruction inst, DisassemblyLine& output)
    {
        output.address = address;
        
        if (inst.word == 0) {
            sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "NOP");
            return;
        }

        switch (inst.opcode())
        {
        case 0x00:
            switch (inst.subfn())
            {
            case 0x00: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SLL    $%u, $%u, %u", inst.regD().i, inst.regT().i, inst.shift()); break;

            case 0x03: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SRA    $%u, $%u, %u", inst.regD().i, inst.regT().i, inst.shift()); break;

            case 0x08: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "JR     $%u", inst.regS().i); break;
            case 0x09: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "JALR   $%u, $%u", inst.regD().i, inst.regS().i); break;

            case 0x12: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "MFLO   $%u", inst.regD().i); break;

            case 0x1A: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "DIV    $%u, $%u", inst.regS().i, inst.regT().i); break;

            case 0x20: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "ADD    $%u, $%u, $%u", inst.regD().i, inst.regS().i, inst.regT().i); break;
            case 0x21: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "ADDU   $%u, $%u, $%u", inst.regD().i, inst.regS().i, inst.regT().i); break;

            case 0x23: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SUBU   $%u, $%u, $%u", inst.regD().i, inst.regS().i, inst.regT().i); break;
            case 0x24: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "AND    $%u, $%u, $%u", inst.regD().i, inst.regS().i, inst.regT().i); break;
            case 0x25: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "OR     $%u, $%u, $%u", inst.regD().i, inst.regS().i, inst.regT().i); break;

            case 0x2B: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SLTU   $%u, $%u, $%u", inst.regD().i, inst.regS().i, inst.regT().i); break;
            default:
                assert(false && "Unhandled opcode!");
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

        case 0x0C: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "ANDI   $%u, $%u, 0x%X", inst.regT().i, inst.regS().i, inst.imm()); break;
        case 0x0D: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "ORI    $%u, $%u, 0x%X", inst.regT().i, inst.regS().i, inst.imm()); break;

        case 0x0F: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "LUI    $%u, 0x%X", inst.regT().i, inst.imm()); break;
        case 0x10:
            switch (inst.copfn())
            {
            case 0x00: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "MFC0   $%u, $%u", inst.regD().i, inst.regT().i); break;
            case 0x04: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "MTC0   $%u, $%u", inst.regD().i, inst.regT().i); break;
            default:
                assert(false && "Unhandled opcode!");
            }
            break;

        case 0x20: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "LB     $%u, 0x%X($%u)", inst.regT().i, inst.imm(), inst.regS().i); break;

        case 0x23: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "LW     $%u, 0x%X($%u)", inst.regT().i, inst.imm(), inst.regS().i); break;
        case 0x24: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "LBU    $%u, 0x%X($%u)", inst.regT().i, inst.imm(), inst.regS().i); break;

        case 0x28: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SB     $%u, 0x%X($%u)", inst.regT().i, inst.imm(), inst.regS().i); break;
        case 0x29: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SH     $%u, 0x%X($%u)", inst.regT().i, inst.imm(), inst.regS().i); break;

        case 0x2B: sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, "SW     $%u, 0x%X($%u)", inst.regT().i, inst.imm(), inst.regS().i); break;
        default:
            assert(false && "Unhandled opcode!");
        }
    }

} // namespace PSX
