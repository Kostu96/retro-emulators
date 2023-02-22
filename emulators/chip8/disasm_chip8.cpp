#include "disasm_chip8.hpp"
#include "chip8_instruction.hpp"
#include "../../shared/asm_common/asm_common.hpp"

#include <cassert>
#include <cstdarg>
#include <iomanip>
#include <sstream>

void disassemble(const u8* code, size_t code_size, std::vector<DisassemblyLine>& output)
{
    for (size_t addr = 0; addr < code_size; )
    {
        DisassemblyLine line;
        line.address = (u16)(addr + 0x200);
        std::stringstream ss;
        ss << std::uppercase << std::hex << std::setw(3) << std::setfill('0') << addr + 0x200 << ":  ";
        Instruction opcode;
        opcode.h2 = code[addr++];
        opcode.h1 = code[addr++];
        ss << std::hex << std::setw(4) << opcode.word << "  ";

        switch (opcode.n4)
        {
        case 0x0:
            if (opcode.word == 0x00E0) ss << "CLS";
            else if (opcode.word == 0x00EE) ss << "RET";
            else ss << "???";
            break;
        case 0x1: ss << "JMP " << (opcode.word & 0xFFF); break;
        case 0x2: ss << "CALL " << (opcode.word & 0xFFF); break;
        case 0x3: ss << "SE V" << (u16)opcode.n3 << ", " << (u16)opcode.h1; break;
        case 0x4: ss << "SNE V" << (u16)opcode.n3 << ", " << (u16)opcode.h1; break;
        case 0x5: ss << "SE V" << (u16)opcode.n3 << ", V" << (u16)opcode.n2; break;
        case 0x6: ss << "LD V" << (u16)opcode.n3 << ", " << (u16)opcode.h1; break;
        case 0x7: ss << "ADD V" << (u16)opcode.n3 << ", " << (u16)opcode.h1; break;
        case 0x8:
            switch (opcode.n1)
            {
            case 0x0: ss << "LD V" << (u16)opcode.n3 << ", V" << (u16)opcode.n2; break;
            case 0x1: ss << "OR V" << (u16)opcode.n3 << ", V" << (u16)opcode.n2; break;
            case 0x2: ss << "AND V" << (u16)opcode.n3 << ", V" << (u16)opcode.n2; break;
            case 0x3: ss << "XOR V" << (u16)opcode.n3 << ", V" << (u16)opcode.n2; break;
            case 0x4: ss << "ADD V" << (u16)opcode.n3 << ", V" << (u16)opcode.n2; break;
            case 0x5: ss << "SUB V" << (u16)opcode.n3 << ", V" << (u16)opcode.n2; break;
            case 0x6: ss << "SHR V" << (u16)opcode.n3; break;
            case 0x7: ss << "SUBN V" << (u16)opcode.n3 << ", V" << (u16)opcode.n2; break;
            case 0xE: ss << "SHL V" << (u16)opcode.n3; break;
            default: ss << "???"; break;
            }
            break;
        case 0x9:
            if (opcode.n1 == 0) ss << "SNE V" << (u16)opcode.n3 << ", V" << (u16)opcode.n2;
            else ss << "???";
            break;
        case 0xA: ss << "LD I, " << (opcode.word & 0xFFF); break;
        case 0xB: ss << "JMP V0, " << (opcode.word & 0xFFF); break;
        case 0xC: ss << "RND V, " << (u16)opcode.n3 << ", " << (u16)opcode.h1; break;
        case 0xD: ss << "SPR V" << (u16)opcode.n3 << ", V" << (u16)opcode.n2 << ", " << (u16)opcode.n1; break;
        case 0xE:
            if (opcode.h1 == 0x9E) ss << "SKP V" << (u16)opcode.n3;
            else if (opcode.h1 == 0xA1) ss << "SKNP V" << (u16)opcode.n3;
            else ss << "???";
            break;
        case 0xF:
            switch (opcode.h1)
            {
            case 0x07: ss << "LD V" << (u16)opcode.n3 << ", DT"; break;
            case 0x0A: ss << "LD V" << (u16)opcode.n3 << ", K"; break;
            case 0x15: ss << "LD DT, V" << (u16)opcode.n3; break;
            case 0x18: ss << "LD ST, V" << (u16)opcode.n3; break;
            case 0x1E: ss << "ADD I, V" << (u16)opcode.n3; break;
            case 0x33: ss << "LD BCD, V" << (u16)opcode.n3; break;
            case 0x55: ss << "LD [I], V" << (u16)opcode.n3; break;
            case 0x65: ss << "LD V" << (u16)opcode.n3 << ", [I]"; break;
            default: ss << "???"; break;
            }
            break;
        default: ss << "???"; break;
        }

        assert(ss.str().size() <= sizeof(line.buffer));
        strcpy_s(line.buffer, ss.str().c_str());
        output.push_back(line);
    }
}
