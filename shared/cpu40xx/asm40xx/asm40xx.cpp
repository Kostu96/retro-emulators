#include "asm40xx.hpp"
#include "parser40xx.hpp"

#include <cassert>
#include <iomanip>

namespace ASM40xx {

    void assemble(const char* source, std::vector<u8>& output)
    {
        Parser parser{ source };

        u32 line = -1;
        parser.advance();
        while(!parser.match(Token::Type::EndOfSource))
            parser.line();

    }

#define PRINT1 printBytes(ss, code, addr, 1, &byte)
#define INST1(str) ss << "    " str
#define INSTR(str, reg) ss << "    " str " " << (reg)
#define INST2(str) do { PRINT1; ss << " " str << std::setw(2) << (u16)byte; } while(false)
#define INST3(str, x) do { PRINT1; ss << " " str " " << (u16)(x) << ", " << std::setw(2) << (u16)byte; } while(false)

    void disassemble(const u8* code, size_t code_size, std::vector<DisassemblyLine>& output)
    {
        for (size_t addr = 0; addr < code_size; )
        {
            DisassemblyLine line;
            line.address = addr;
            std::stringstream ss;
            ss << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << addr << ":  ";
            u8 opcode = code[addr++];
            ss << std::hex << std::setw(2) << (u16)opcode << ' ';

            u8 byte;
            switch (opcode >> 4)
            {
            case 0x0:
                if (opcode & 0xF)
                    INST1("???");
                else
                    INST1("NOP");
                break;
            case 0x1: INST3("JCN", opcode & 0xF); break;
            case 0x2:
                if (opcode & 1)
                    INSTR("SRC", (opcode & 0xE) / 2);
                else
                    INST3("FIM", (opcode & 0xE) / 2);
                break;
            case 0x3:
                if (opcode & 1)
                    INSTR("JIN", (opcode & 0xE) / 2);
                else
                    INSTR("FIN", (opcode & 0xE) / 2);
                break;
            case 0x4: PRINT1; ss << " JUN " << std::setw(3) << (((u16)opcode & 0xF) << 8 | byte); break;
            case 0x5: PRINT1; ss << " JMS " << std::setw(3) << (((u16)opcode & 0xF) << 8 | byte); break;
            case 0x6: INSTR("INC", opcode & 0xF); break;
            case 0x7: INST3("ISZ", opcode & 0xF); break;
            case 0x8: INSTR("ADD", opcode & 0xF); break;
            case 0x9: INSTR("SUB", opcode & 0xF); break;
            case 0xA: INSTR("LD ", opcode & 0xF); break;
            case 0xB: INSTR("XCH", opcode & 0xF); break;
            case 0xC: INSTR("BBL", opcode & 0xF); break;
            case 0xD: INSTR("LDM", opcode & 0xF); break;
            case 0xE:
                switch (opcode & 0xF)
                {
                case 0x0: INST1("WRM"); break;
                case 0x1: INST1("WMP"); break;
                case 0x2: INST1("WRR"); break;
                case 0x4: INST1("WR0"); break;
                case 0x5: INST1("WR1"); break;
                case 0x6: INST1("WR2"); break;
                case 0x7: INST1("WR3"); break;
                case 0x8: INST1("SBM"); break;
                case 0x9: INST1("RDM"); break;
                case 0xA: INST1("RDR"); break;
                case 0xB: INST1("ADM"); break;
                case 0xC: INST1("RD0"); break;
                case 0xD: INST1("RD1"); break;
                case 0xE: INST1("RD2"); break;
                case 0xF: INST1("RD3"); break;
                default: INST1("???");
                }
                break;
            case 0xF:
                switch (opcode & 0xF)
                {
                case 0x0: INST1("CLB"); break;
                case 0x1: INST1("CLC"); break;
                case 0x2: INST1("IAC"); break;
                case 0x3: INST1("CMC"); break;
                case 0x4: INST1("CMA"); break;
                case 0x5: INST1("RAL"); break;
                case 0x6: INST1("RAR"); break;
                case 0x7: INST1("TCC"); break;
                case 0x8: INST1("DAC"); break;
                case 0x9: INST1("TCS"); break;
                case 0xA: INST1("STC"); break;
                case 0xB: INST1("DAA"); break;
                case 0xC: INST1("KBP"); break;
                case 0xD: INST1("DCL"); break;
                default: INST1("???");
                }
                break;
            default: INST1("???");
            }

            assert(ss.str().size() <= sizeof(line.buffer));
            strcpy_s(line.buffer, ss.str().c_str());
            output.push_back(line);
        }
    }

}
