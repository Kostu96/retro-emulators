#include "shared/source/devices/cpu8080/disasm8080.hpp"
#include "shared/source/disassembly_line.hpp"

#include <cassert>
#include <iomanip>

#define PRINT1 printBytes(ss, code, addr, 1, &byte1)
#define PRINT2 printBytes(ss, code, addr, 2, &byte1, &byte2)

void disassemble(const u8* code, size_t code_size, std::vector<DisassemblyLine>& output)
{
#define INST1(str) ss << "       " str; break
#define INST2(str) PRINT1; ss << "    " str << std::setw(2) << (u16)byte1; break
#define INST3(str) PRINT2; ss << " " str << std::setw(2) << (u16)byte2 << std::setw(2) << (u16)byte1; break

    for (size_t addr = 0; addr < code_size; )
    {
        DisassemblyLine line;
        line.address = (u16)addr;
        std::stringstream ss;
        ss << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << addr << ":  ";
        u8 byte = code[addr++];
        ss << std::hex << std::setw(2) << (u16)byte << ' ';

        u8 byte1, byte2;
        switch (byte)
        {
        case 0x00: INST1("NOP");
        case 0x01: INST3("LXI BC, ");
        case 0x02: INST1("STAX BC");
        case 0x03: INST1("INC BC");
        case 0x04: INST1("INC B");
        case 0x05: INST1("DEC B");
        case 0x06: INST2("MVI B, ");
        case 0x07: INST1("RLC");

        case 0x09: INST1("DAD BC");
        case 0x0A: INST1("LDAX BC");
        case 0x0B: INST1("DEC BC");
        case 0x0C: INST1("INC C");
        case 0x0D: INST1("DEC C");
        case 0x0E: INST2("MVI C, ");
        case 0x0F: INST1("RRC");

        case 0x11: INST3("LXI DE, ");
        case 0x12: INST1("STAX DE");
        case 0x13: INST1("INC DE");
        case 0x14: INST1("INC D");
        case 0x15: INST1("DEC D");
        case 0x16: INST2("MVI D, ");
        case 0x17: INST1("RAL");

        case 0x19: INST1("DAD DE");
        case 0x1A: INST1("LDAX DE");
        case 0x1B: INST1("DEC DE");
        case 0x1C: INST1("INC E");
        case 0x1D: INST1("DEC E");
        case 0x1E: INST2("MVI E, ");
        case 0x1F: INST1("RAR");

        case 0x21: INST3("LXI HL, ");
        case 0x22: INST3("SHLD ");
        case 0x23: INST1("INC HL");
        case 0x24: INST1("INC H");
        case 0x25: INST1("DEC H");
        case 0x26: INST2("MVI H, ");
        case 0x27: INST1("DAA");

        case 0x29: INST1("DAD HL");
        case 0x2A: INST3("LHLD ");
        case 0x2B: INST1("DEC HL");
        case 0x2C: INST1("INC L");
        case 0x2D: INST1("DEC L");
        case 0x2E: INST2("MVI L, ");
        case 0x2F: INST1("CMA");

        case 0x31: INST3("LXI SP, ");
        case 0x32: INST3("STA ");
        case 0x33: INST1("INC SP");
        case 0x34: INST1("INC M");
        case 0x35: INST1("DEC M");
        case 0x36: INST2("MVI M, ");
        case 0x37: INST1("STC");

        case 0x39: INST1("DAD SP");
        case 0x3A: INST3("LDA ");
        case 0x3B: INST1("DEC SP");
        case 0x3C: INST1("INC A");
        case 0x3D: INST1("DEC A");
        case 0x3E: INST2("MVI A, ");
        case 0x3F: INST1("CMC");
        case 0x40: INST1("MOV B, B");
        case 0x41: INST1("MOV B, C");
        case 0x42: INST1("MOV B, D");
        case 0x43: INST1("MOV B, E");
        case 0x44: INST1("MOV B, H");
        case 0x45: INST1("MOV B, L");
        case 0x46: INST1("MOV B, M");
        case 0x47: INST1("MOV B, A");
        case 0x48: INST1("MOV C, B");
        case 0x49: INST1("MOV C, C");
        case 0x4A: INST1("MOV C, D");
        case 0x4B: INST1("MOV C, E");
        case 0x4C: INST1("MOV C, H");
        case 0x4D: INST1("MOV C, L");
        case 0x4E: INST1("MOV C, M");
        case 0x4F: INST1("MOV C, A");
        case 0x50: INST1("MOV D, B");
        case 0x51: INST1("MOV D, C");
        case 0x52: INST1("MOV D, D");
        case 0x53: INST1("MOV D, E");
        case 0x54: INST1("MOV D, H");
        case 0x55: INST1("MOV D, L");
        case 0x56: INST1("MOV D, M");
        case 0x57: INST1("MOV D, A");
        case 0x58: INST1("MOV E, B");
        case 0x59: INST1("MOV E, C");
        case 0x5A: INST1("MOV E, D");
        case 0x5B: INST1("MOV E, E");
        case 0x5C: INST1("MOV E, H");
        case 0x5D: INST1("MOV E, L");
        case 0x5E: INST1("MOV E, M");
        case 0x5F: INST1("MOV E, A");
        case 0x60: INST1("MOV H, B");
        case 0x61: INST1("MOV H, C");
        case 0x62: INST1("MOV H, D");
        case 0x63: INST1("MOV H, E");
        case 0x64: INST1("MOV H, H");
        case 0x65: INST1("MOV H, L");
        case 0x66: INST1("MOV H, M");
        case 0x67: INST1("MOV H, A");
        case 0x68: INST1("MOV L, B");
        case 0x69: INST1("MOV L, C");
        case 0x6A: INST1("MOV L, D");
        case 0x6B: INST1("MOV L, E");
        case 0x6C: INST1("MOV L, H");
        case 0x6D: INST1("MOV L, L");
        case 0x6E: INST1("MOV L, M");
        case 0x6F: INST1("MOV L, A");
        case 0x70: INST1("MOV M, B");
        case 0x71: INST1("MOV M, C");
        case 0x72: INST1("MOV M, D");
        case 0x73: INST1("MOV M, E");
        case 0x74: INST1("MOV M, H");
        case 0x75: INST1("MOV M, L");
        case 0x76: INST1("HLT");
        case 0x77: INST1("MOV M, A");
        case 0x78: INST1("MOV A, B");
        case 0x79: INST1("MOV A, C");
        case 0x7A: INST1("MOV A, D");
        case 0x7B: INST1("MOV A, E");
        case 0x7C: INST1("MOV A, H");
        case 0x7D: INST1("MOV A, L");
        case 0x7E: INST1("MOV A, M");
        case 0x7F: INST1("MOV A, A");
        case 0x80: INST1("ADD B");
        case 0x81: INST1("ADD C");
        case 0x82: INST1("ADD D");
        case 0x83: INST1("ADD E");
        case 0x84: INST1("ADD H");
        case 0x85: INST1("ADD L");
        case 0x86: INST1("ADD M");
        case 0x87: INST1("ADD A");
        case 0x88: INST1("ADC B");
        case 0x89: INST1("ADC C");
        case 0x8A: INST1("ADC D");
        case 0x8B: INST1("ADC E");
        case 0x8C: INST1("ADC H");
        case 0x8D: INST1("ADC L");
        case 0x8E: INST1("ADC M");
        case 0x8F: INST1("ADC A");
        case 0x90: INST1("SUB B");
        case 0x91: INST1("SUB C");
        case 0x92: INST1("SUB D");
        case 0x93: INST1("SUB E");
        case 0x94: INST1("SUB H");
        case 0x95: INST1("SUB L");
        case 0x96: INST1("SUB M");
        case 0x97: INST1("SUB A");
        case 0x98: INST1("SBB B");
        case 0x99: INST1("SBB C");
        case 0x9A: INST1("SBB D");
        case 0x9B: INST1("SBB E");
        case 0x9C: INST1("SBB H");
        case 0x9D: INST1("SBB L");
        case 0x9E: INST1("SBB M");
        case 0x9F: INST1("SBB A");
        case 0xA0: INST1("ANA B");
        case 0xA1: INST1("ANA C");
        case 0xA2: INST1("ANA D");
        case 0xA3: INST1("ANA E");
        case 0xA4: INST1("ANA H");
        case 0xA5: INST1("ANA L");
        case 0xA6: INST1("ANA M");
        case 0xA7: INST1("ANA A");
        case 0xA8: INST1("XRA B");
        case 0xA9: INST1("XRA C");
        case 0xAA: INST1("XRA D");
        case 0xAB: INST1("XRA E");
        case 0xAC: INST1("XRA H");
        case 0xAD: INST1("XRA L");
        case 0xAE: INST1("XRA M");
        case 0xAF: INST1("XRA A");
        case 0xB0: INST1("ORA B");
        case 0xB1: INST1("ORA C");
        case 0xB2: INST1("ORA D");
        case 0xB3: INST1("ORA E");
        case 0xB4: INST1("ORA H");
        case 0xB5: INST1("ORA L");
        case 0xB6: INST1("ORA M");
        case 0xB7: INST1("ORA A");
        case 0xB8: INST1("CMP B");
        case 0xB9: INST1("CMP C");
        case 0xBA: INST1("CMP D");
        case 0xBB: INST1("CMP E");
        case 0xBC: INST1("CMP H");
        case 0xBD: INST1("CMP L");
        case 0xBE: INST1("CMP M");
        case 0xBF: INST1("CMP A");
        case 0xC0: INST1("RNZ");
        case 0xC1: INST1("POP BC");
        case 0xC2: INST3("JNZ ");
        case 0xC3: INST3("JMP ");
        case 0xC4: INST3("CNZ ");
        case 0xC5: INST1("PUSH BC");
        case 0xC6: INST2("ADI ");
        case 0xC7: INST1("RST 0");
        case 0xC8: INST1("RZ");
        case 0xC9: INST1("RET");
        case 0xCA: INST3("JZ ");

        case 0xCC: INST3("CZ ");
        case 0xCD: INST3("CALL ");
        case 0xCE: INST2("ACI ");
        case 0xCF: INST1("RST 1");
        case 0xD0: INST1("RNC");
        case 0xD1: INST1("POP DE");
        case 0xD2: INST3("JNC ");
        case 0xD3: INST2("OUT ");
        case 0xD4: INST3("CNC ");
        case 0xD5: INST1("PUSH DE");
        case 0xD6: INST2("SUI ");
        case 0xD7: INST1("RST 2");
        case 0xD8: INST1("RC");

        case 0xDA: INST3("JC ");
        case 0xDB: INST2("IN ");
        case 0xDC: INST3("CC ");
        case 0xDE: INST2("SBI ");
        case 0xDF: INST1("RST 3");
        case 0xE0: INST1("RPO");
        case 0xE1: INST1("POP HL");
        case 0xE2: INST3("JPO ");
        case 0xE3: INST1("XTHL");
        case 0xE4: INST3("CPO ");
        case 0xE5: INST1("PUSH HL");
        case 0xE6: INST2("ANI ");
        case 0xE7: INST1("RST 4");
        case 0xE8: INST1("RPE");
        case 0xE9: INST1("PCHL");
        case 0xEA: INST3("JPE ");
        case 0xEB: INST1("XCHG");
        case 0xEC: INST3("CPE ");

        case 0xEE: INST2("XRI ");
        case 0xEF: INST1("RST 5");
        case 0xF0: INST1("RP");
        case 0xF1: INST1("POP AF");
        case 0xF2: INST3("JP ");
        case 0xF3: INST1("DI");
        case 0xF4: INST3("CP ");
        case 0xF5: INST1("PUSH AF");
        case 0xF6: INST2("ORI ");
        case 0xF7: INST1("RST 6");
        case 0xF8: INST1("RM");
        case 0xF9: INST1("SPHL");
        case 0xFA: INST3("JM ");
        case 0xFB: INST1("EI");
        case 0xFC: INST3("CM ");

        case 0xFE: INST2("CPI ");
        case 0xFF: INST1("RST 7");
        default: INST1("???");
        }

        assert(ss.str().size() <= sizeof(line.buffer));
        strcpy_s(line.buffer, ss.str().c_str());
        output.push_back(line);
    }

#undef INST1
#undef INST2
#undef INST3
}

void disasmIntruction(u8 opcode, u8 byte1, u8 byte2, DisassemblyLine& output)
{
#define INST1(mnemonic) sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, mnemonic)
#define INST2(mnemonic) sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, mnemonic " 0x%02X", byte1)
#define INSTW(mnemonic) sprintf_s(output.buffer, DisassemblyLine::BUFFER_SIZE, mnemonic " 0x%04X", (u16)byte2 << 8 | byte1)

    switch (opcode)
    {
    case 0x00: INST1("NOP"); break;
    case 0x01: INSTW("LXI BC,"); break;

    case 0x05: INST1("DCR B"); break;
    case 0x06: INST2("MVI B,"); break;

    case 0x09: INST1("DAD BC"); break;

    case 0x0D: INST1("DCR C"); break;
    case 0x0E: INST2("MVI C,"); break;
    case 0x0F: INST1("RRC"); break;

    case 0x11: INSTW("LXI DE,"); break;

    case 0x13: INST1("INX DE"); break;

    case 0x19: INST1("DAD DE"); break;
    case 0x1A: INST1("LDAX"); break;

    case 0x21: INSTW("LXI HL,"); break;

    case 0x23: INST1("INX HL"); break;

    case 0x26: INST2("MVI H,"); break;

    case 0x29: INST1("DAD HL"); break;

    case 0x31: INSTW("LXI SP,"); break;
    case 0x32: INSTW("STA"); break;

    case 0x36: INST2("MVI (HL),"); break;

    case 0x3A: INSTW("LDA"); break;

    case 0x3E: INST2("MVI A,"); break;

    case 0x40: INST1("MOV B, B"); break;
    case 0x41: INST1("MOV B, C"); break;
    case 0x42: INST1("MOV B, D"); break;
    case 0x43: INST1("MOV B, E"); break;
    case 0x44: INST1("MOV B, H"); break;
    case 0x45: INST1("MOV B, L"); break;
    case 0x46: INST1("MOV B, (HL)"); break;
    case 0x47: INST1("MOV B, A"); break;
    case 0x48: INST1("MOV C, B"); break;
    case 0x49: INST1("MOV C, C"); break;
    case 0x4A: INST1("MOV C, D"); break;
    case 0x4B: INST1("MOV C, E"); break;
    case 0x4C: INST1("MOV C, H"); break;
    case 0x4D: INST1("MOV C, L"); break;
    case 0x4E: INST1("MOV C, (HL)"); break;
    case 0x4F: INST1("MOV C, A"); break;
    case 0x50: INST1("MOV D, B"); break;
    case 0x51: INST1("MOV D, C"); break;
    case 0x52: INST1("MOV D, D"); break;
    case 0x53: INST1("MOV D, E"); break;
    case 0x54: INST1("MOV D, H"); break;
    case 0x55: INST1("MOV D, L"); break;
    case 0x56: INST1("MOV D, (HL)"); break;
    case 0x57: INST1("MOV D, A"); break;
    case 0x58: INST1("MOV E, B"); break;
    case 0x59: INST1("MOV E, C"); break;
    case 0x5A: INST1("MOV E, D"); break;
    case 0x5B: INST1("MOV E, E"); break;
    case 0x5C: INST1("MOV E, H"); break;
    case 0x5D: INST1("MOV E, L"); break;
    case 0x5E: INST1("MOV E, (HL)"); break;
    case 0x5F: INST1("MOV E, A"); break;
    case 0x60: INST1("MOV H, B"); break;
    case 0x61: INST1("MOV H, C"); break;
    case 0x62: INST1("MOV H, D"); break;
    case 0x63: INST1("MOV H, E"); break;
    case 0x64: INST1("MOV H, H"); break;
    case 0x65: INST1("MOV H, L"); break;
    case 0x66: INST1("MOV H, (HL)"); break;
    case 0x67: INST1("MOV H, A"); break;
    case 0x68: INST1("MOV L, B"); break;
    case 0x69: INST1("MOV L, C"); break;
    case 0x6A: INST1("MOV L, D"); break;
    case 0x6B: INST1("MOV L, E"); break;
    case 0x6C: INST1("MOV L, H"); break;
    case 0x6D: INST1("MOV L, L"); break;
    case 0x6E: INST1("MOV L, (HL)"); break;
    case 0x6F: INST1("MOV L, A"); break;
    case 0x70: INST1("MOV (HL), B"); break;
    case 0x71: INST1("MOV (HL), C"); break;
    case 0x72: INST1("MOV (HL), D"); break;
    case 0x73: INST1("MOV (HL), E"); break;
    case 0x74: INST1("MOV (HL), H"); break;
    case 0x75: INST1("MOV (HL), L"); break;
    case 0x76: INST1("HLT"); break;
    case 0x77: INST1("MOV (HL), A"); break;
    case 0x78: INST1("MOV A, B"); break;
    case 0x79: INST1("MOV A, C"); break;
    case 0x7A: INST1("MOV A, D"); break;
    case 0x7B: INST1("MOV A, E"); break;
    case 0x7C: INST1("MOV A, H"); break;
    case 0x7D: INST1("MOV A, L"); break;
    case 0x7E: INST1("MOV A, (HL)"); break;
    case 0x7F: INST1("MOV A, A"); break;
    case 0x80: INST1("ADD B"); break;
    case 0x81: INST1("ADD C"); break;
    case 0x82: INST1("ADD D"); break;
    case 0x83: INST1("ADD E"); break;
    case 0x84: INST1("ADD H"); break;
    case 0x85: INST1("ADD L"); break;
    case 0x86: INST1("ADD (HL)"); break;
    case 0x87: INST1("ADD A"); break;
    case 0x88: INST1("ADC B"); break;
    case 0x89: INST1("ADC C"); break;
    case 0x8A: INST1("ADC D"); break;
    case 0x8B: INST1("ADC E"); break;
    case 0x8C: INST1("ADC H"); break;
    case 0x8D: INST1("ADC L"); break;
    case 0x8E: INST1("ADC (HL)"); break;
    case 0x8F: INST1("ADC A"); break;
    case 0x90: INST1("SUB B"); break;
    case 0x91: INST1("SUB C"); break;
    case 0x92: INST1("SUB D"); break;
    case 0x93: INST1("SUB E"); break;
    case 0x94: INST1("SUB H"); break;
    case 0x95: INST1("SUB L"); break;
    case 0x96: INST1("SUB (HL)"); break;
    case 0x97: INST1("SUB A"); break;
    case 0x98: INST1("SBB B"); break;
    case 0x99: INST1("SBB C"); break;
    case 0x9A: INST1("SBB D"); break;
    case 0x9B: INST1("SBB E"); break;
    case 0x9C: INST1("SBB H"); break;
    case 0x9D: INST1("SBB L"); break;
    case 0x9E: INST1("SBB (HL)"); break;
    case 0x9F: INST1("SBB A"); break;
    case 0xA0: INST1("AND B"); break;
    case 0xA1: INST1("AND C"); break;
    case 0xA2: INST1("AND D"); break;
    case 0xA3: INST1("AND E"); break;
    case 0xA4: INST1("AND H"); break;
    case 0xA5: INST1("AND L"); break;
    case 0xA6: INST1("AND (HL)"); break;
    case 0xA7: INST1("AND A"); break;
    case 0xA8: INST1("XOR B"); break;
    case 0xA9: INST1("XOR C"); break;
    case 0xAA: INST1("XOR D"); break;
    case 0xAB: INST1("XOR E"); break;
    case 0xAC: INST1("XOR H"); break;
    case 0xAD: INST1("XOR L"); break;
    case 0xAE: INST1("XOR (HL)"); break;
    case 0xAF: INST1("XOR A"); break;
    case 0xB0: INST1("OR B"); break;
    case 0xB1: INST1("OR C"); break;
    case 0xB2: INST1("OR D"); break;
    case 0xB3: INST1("OR E"); break;
    case 0xB4: INST1("OR H"); break;
    case 0xB5: INST1("OR L"); break;
    case 0xB6: INST1("OR (HL)"); break;
    case 0xB7: INST1("OR A"); break;
    case 0xB8: INST1("CMP B"); break;
    case 0xB9: INST1("CMP C"); break;
    case 0xBA: INST1("CMP D"); break;
    case 0xBB: INST1("CMP E"); break;
    case 0xBC: INST1("CMP H"); break;
    case 0xBD: INST1("CMP L"); break;
    case 0xBE: INST1("CMP (HL)"); break;
    case 0xBF: INST1("CMP A"); break;

    case 0xC1: INST1("POP BC"); break;
    case 0xC2: INSTW("JNZ"); break;
    case 0xC3: INSTW("JMP"); break;

    case 0xC5: INST1("PUSH BC"); break;
    case 0xC6: INST2("ADI"); break;

    case 0xC9: INST1("RET"); break;

    case 0xCD: INSTW("CALL"); break;

    case 0xD1: INST1("POP DE"); break;

    case 0xD3: INST2("OUT"); break;
    case 0xD4: INSTW("CNC"); break;
    case 0xD5: INST1("PUSH DE"); break;

    case 0xE1: INST1("POP HL"); break;

    case 0xE5: INST1("PUSH HL"); break;
    case 0xE6: INST2("ANI"); break;

    case 0xEB: INST1("XCHG"); break;

    case 0xF1: INST1("RP"); break;

    case 0xF5: INST1("PUSH AF"); break;

    case 0xFB: INST1("EI"); break;

    case 0xFE: INST2("CPI"); break;

    default:
        assert(false);
        break;
    }

#undef INST1
#undef INST2
#undef INSTW
}
