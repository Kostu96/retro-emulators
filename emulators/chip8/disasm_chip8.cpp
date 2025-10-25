#include "disasm_chip8.hpp"
#include "chip8_instruction.hpp"
#include "utils/asm_common.hpp"
#include "utils/disassembly_line.hpp"

#include <cassert>
#include <cstdarg>
#include <iomanip>
#include <sstream>
#include <cstring>

#define PRINT_HEX(val, w) std::setw(w) << to_u16(val)

void disassemble(const u8* code, size_t code_size, std::vector<DisassemblyLine>& output)
{
    for (size_t addr = 0; addr < code_size; )
    {
        DisassemblyLine line;
        line.address = to_u16(addr + 0x200);
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
        case 0x1: ss << "JMP "  << (opcode.word & 0xFFF); break;
        case 0x2: ss << "CALL " << (opcode.word & 0xFFF); break;
        case 0x3: ss << "SEQ V" << to_u16(opcode.n3) << ", " << PRINT_HEX(opcode.h1, 2); break;
        case 0x4: ss << "SNE V" << to_u16(opcode.n3) << ", " << PRINT_HEX(opcode.h1, 2); break;
        case 0x5:
            if (opcode.n1 == 0)ss << "SEQ V"  << to_u16(opcode.n3) << ", V" << to_u16(opcode.n2);
            else ss << "???";
            break;
        case 0x6: ss << "LD V"  << to_u16(opcode.n3) << ", " << PRINT_HEX(opcode.h1, 2); break;
        case 0x7: ss << "ADD V" << to_u16(opcode.n3) << ", " << PRINT_HEX(opcode.h1, 2); break;
        case 0x8:
            switch (opcode.n1)
            {
            case 0x0: ss << "LD V"  << to_u16(opcode.n3) << ", V" << to_u16(opcode.n2); break;
            case 0x1: ss << "OR V"  << to_u16(opcode.n3) << ", V" << to_u16(opcode.n2); break;
            case 0x2: ss << "AND V" << to_u16(opcode.n3) << ", V" << to_u16(opcode.n2); break;
            case 0x3: ss << "XOR V" << to_u16(opcode.n3) << ", V" << to_u16(opcode.n2); break;
            case 0x4: ss << "ADD V" << to_u16(opcode.n3) << ", V" << to_u16(opcode.n2); break;
            case 0x5: ss << "SUB V" << to_u16(opcode.n3) << ", V" << to_u16(opcode.n2); break;
            case 0x6: ss << "SHR V" << to_u16(opcode.n3); break;
            case 0x7: ss << "SUBN V" << to_u16(opcode.n3) << ", V" << to_u16(opcode.n2); break;
            case 0xE: ss << "SHL V" << to_u16(opcode.n3); break;
            default: ss << "???"; break;
            }
            break;
        case 0x9:
            if (opcode.n1 == 0) ss << "SNE V" << to_u16(opcode.n3) << ", V" << to_u16(opcode.n2);
            else ss << "???";
            break;
        case 0xA: ss << "LD I, "   << (opcode.word & 0xFFF); break;
        case 0xB: ss << "JMP V0, " << (opcode.word & 0xFFF); break;
        case 0xC: ss << "RND V" << to_u16(opcode.n3) << ", " << PRINT_HEX(opcode.h1, 2); break;
        case 0xD: ss << "SPR V" << to_u16(opcode.n3) << ", V" << to_u16(opcode.n2) << ", " << to_u16(opcode.n1); break;
        case 0xE:
            if (opcode.h1 == 0x9E) ss << "SKP V" << to_u16(opcode.n3);
            else if (opcode.h1 == 0xA1) ss << "SKNP V" << to_u16(opcode.n3);
            else ss << "???";
            break;
        case 0xF:
            switch (opcode.h1)
            {
            case 0x07: ss << "LD V" << to_u16(opcode.n3) << ", DT"; break;
            case 0x0A: ss << "LD V" << to_u16(opcode.n3) << ", K"; break;
            case 0x15: ss << "LD DT, V" << to_u16(opcode.n3); break;
            case 0x18: ss << "LD ST, V" << to_u16(opcode.n3); break;
            case 0x1E: ss << "ADD I, V" << to_u16(opcode.n3); break;
            case 0x29: ss << "CHR I, V" << to_u16(opcode.n3); break;
            case 0x33: ss << "LD BCD, V" << to_u16(opcode.n3); break;
            case 0x55: ss << "LD [I], V" << to_u16(opcode.n3); break;
            case 0x65: ss << "LD V" << to_u16(opcode.n3) << ", [I]"; break;
            default: ss << "???"; break;
            }
            break;
        default: ss << "???"; break;
        }

        strncpy(line.buffer, ss.str().c_str(), sizeof(line.buffer));
        output.push_back(line);
    }
}

#undef PRINT_HEX
