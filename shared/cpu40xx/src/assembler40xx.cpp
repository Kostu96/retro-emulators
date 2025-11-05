#include "cpu40xx/assembler40xx.hpp"
#include "utils/disassembly_line.hpp"

#include <cassert>
#include <charconv>
#include <cstring>
#include <format>
#include <iomanip>
#include <iostream>

namespace {

//constexpr std::array<Mnemonic, 46> mnemonics = {
//    Mnemonic{ "ADD", 1, 0x80, Mnemonic::Arg::Register },
//    Mnemonic{ "ADM", 1, 0xEB, Mnemonic::Arg::None },
//    Mnemonic{ "BBL", 1, 0xC0, Mnemonic::Arg::Immediate4 },
//    Mnemonic{ "CLB", 1, 0xF0, Mnemonic::Arg::None },
//    Mnemonic{ "CLC", 1, 0xF1, Mnemonic::Arg::None },
//    Mnemonic{ "CMA", 1, 0xF4, Mnemonic::Arg::None },
//    Mnemonic{ "CMC", 1, 0xF3, Mnemonic::Arg::None },
//    Mnemonic{ "DAA", 1, 0xFB, Mnemonic::Arg::None },
//    Mnemonic{ "DAC", 1, 0xF8, Mnemonic::Arg::None },
//    Mnemonic{ "DCL", 1, 0xFD, Mnemonic::Arg::None },
//    Mnemonic{ "FIM", 2, 0x20, Mnemonic::Arg::RegisterPairImmediate8 },
//    Mnemonic{ "FIN", 1, 0x30, Mnemonic::Arg::RegisterPair },
//    Mnemonic{ "IAC", 1, 0xF2, Mnemonic::Arg::None },
//    Mnemonic{ "INC", 1, 0x60, Mnemonic::Arg::Register },
//    Mnemonic{ "ISZ", 2, 0x70, Mnemonic::Arg::RegisterPairImmediate8 },
//    Mnemonic{ "JCN", 2, 0x10, Mnemonic::Arg::ConditionImmediate8 },
//    Mnemonic{ "JIN", 1, 0x31, Mnemonic::Arg::RegisterPair },
//    Mnemonic{ "JMS", 2, 0x50, Mnemonic::Arg::Immediate12 },
//    Mnemonic{ "JUN", 2, 0x40, Mnemonic::Arg::Immediate12 },
//    Mnemonic{ "KBP", 1, 0xFC, Mnemonic::Arg::None },
//    Mnemonic{ "LD",  1, 0xA0, Mnemonic::Arg::Register },
//    Mnemonic{ "LDM", 1, 0xD0, Mnemonic::Arg::Immediate4 },
//    Mnemonic{ "NOP", 1, 0x00, Mnemonic::Arg::None },
//    Mnemonic{ "RAL", 1, 0xF5, Mnemonic::Arg::None },
//    Mnemonic{ "RAR", 1, 0xF6, Mnemonic::Arg::None },
//    Mnemonic{ "RD0", 1, 0xEC, Mnemonic::Arg::None },
//    Mnemonic{ "RD1", 1, 0xED, Mnemonic::Arg::None },
//    Mnemonic{ "RD2", 1, 0xEE, Mnemonic::Arg::None },
//    Mnemonic{ "RD3", 1, 0xEF, Mnemonic::Arg::None },
//    Mnemonic{ "RDM", 1, 0xE9, Mnemonic::Arg::None },
//    Mnemonic{ "RDR", 1, 0xEA, Mnemonic::Arg::None },
//    Mnemonic{ "SBM", 1, 0xE8, Mnemonic::Arg::None },
//    Mnemonic{ "SRC", 1, 0x21, Mnemonic::Arg::RegisterPair },
//    Mnemonic{ "STC", 1, 0xFA, Mnemonic::Arg::None },
//    Mnemonic{ "SUB", 1, 0x90, Mnemonic::Arg::Register },
//    Mnemonic{ "TCC", 1, 0xF7, Mnemonic::Arg::None },
//    Mnemonic{ "TCS", 1, 0xF9, Mnemonic::Arg::None },
//    Mnemonic{ "WMP", 1, 0xE1, Mnemonic::Arg::None },
//    Mnemonic{ "WR0", 1, 0xE4, Mnemonic::Arg::None },
//    Mnemonic{ "WR1", 1, 0xE5, Mnemonic::Arg::None },
//    Mnemonic{ "WR2", 1, 0xE6, Mnemonic::Arg::None },
//    Mnemonic{ "WR3", 1, 0xE7, Mnemonic::Arg::None },
//    Mnemonic{ "WRM", 1, 0xE0, Mnemonic::Arg::None },
//    Mnemonic{ "WRR", 1, 0xE2, Mnemonic::Arg::None },
//    Mnemonic{ "XCH", 1, 0xB0, Mnemonic::Arg::Register }
//};

//constexpr std::optional<std::reference_wrapper<const Mnemonic>> find_mnemonic(std::string_view key) {
//    for (auto& e : mnemonics) {
//        if (e.key == key) {
//            return std::cref(e);
//        }
//    }
//    return std::nullopt;
//}

} // namespace

Assembler40xx::Assembler40xx(std::string_view source) :
    m_source(source) {
    m_labels.emplace("R0", to_u16(0));
    m_labels.emplace("R1", to_u16(1));
    m_labels.emplace("R2", to_u16(2));
    m_labels.emplace("R3", to_u16(3));
    m_labels.emplace("R4", to_u16(4));
    m_labels.emplace("R5", to_u16(5));
    m_labels.emplace("R6", to_u16(6));
    m_labels.emplace("R7", to_u16(7));
    m_labels.emplace("R8", to_u16(8));
    m_labels.emplace("R9", to_u16(9));
    m_labels.emplace("R10", to_u16(10));
    m_labels.emplace("R11", to_u16(11));
    m_labels.emplace("R12", to_u16(12));
    m_labels.emplace("R13", to_u16(13));
    m_labels.emplace("R14", to_u16(14));
    m_labels.emplace("R15", to_u16(15));
    m_labels.emplace("P0", to_u16(0));
    m_labels.emplace("P1", to_u16(2));
    m_labels.emplace("P2", to_u16(4));
    m_labels.emplace("P3", to_u16(6));
    m_labels.emplace("P4", to_u16(8));
    m_labels.emplace("P5", to_u16(10));
    m_labels.emplace("P6", to_u16(12));
    m_labels.emplace("P7", to_u16(14));
}

Assembler40xx::Status Assembler40xx::assemble() {
    size_t start = 0;
    u32 lineNumber = 1;
    while (start < m_source.size()) {
        size_t endLine = m_source.find('\n', start);
        size_t comment = m_source.find('/', start);
        size_t end = std::min(endLine, comment);
        if (end == std::string_view::npos) {
            m_lines.emplace_back(Line{
                .str = m_source.substr(start),
                .lineNumber = lineNumber
            });
            break;
        }

        // trim optional carriage return (\r) for Windows CRLF
        size_t lineLength = end - start;
        if (lineLength > 0 && m_source[start + lineLength - 1] == '\r') {
            --lineLength;
        }

        if (lineLength > 0) {
            auto line = m_source.substr(start, lineLength);
            size_t firstNonBlank = line.find_first_not_of(' ');
            size_t lastNonBlank = line.find_last_not_of(' ');
            m_lines.emplace_back(Line{
                .str = line.substr(firstNonBlank, lastNonBlank - firstNonBlank + 1),
                .lineNumber = lineNumber
            });
        }

        start = endLine + 1;
        lineNumber++;
    }

    m_address = 0;

    for (auto& l : m_lines) {
        parseLine1stPass(l);
    }

    // Remove line with size == 0 (origin and equate)
    std::erase_if(m_lines, [](auto& e) { return e.size == 0; });

    for (auto& l : m_lines) {
        if (!l.isComplete) {
            parseLine2ndPass(l);
        }
    }

    std::stringstream hexStream;
    for (auto& l : m_lines) {
        hexStream << ':';                               // start code
        hexStream << "01";                              // byte count
        hexStream << std::format("{:04X}", l.address);  // address
        hexStream << "00";                              // record type
        hexStream << std::format("{:02X}", l.bytes[0]); // data
        hexStream << "FF";                              // checksum
        hexStream << '\n';
    }
    hexStream << ":00000001FF";
    m_hex = hexStream.str();

    return hasError ? Status::Error : Status::Success;
}

std::string Assembler40xx::getLog() const {
    // TODO(Kostu): implement
    return std::string();
}

void Assembler40xx::parseLine1stPass(Line& line) {
    line.address = m_address;
    std::string_view str = line.str;
    bool shouldBreak = false;
    while (true) {
        size_t pos = str.find_first_of(' ');
        std::string_view token = str.substr(0, pos);

        //std::cout << "  [" << token << "]\n";

        char last = token.back();
        switch (last) {
        case ',': { // Label
            m_labels.emplace(token.substr(0, token.size() - 1), m_address);
        } break;
        case '=': {
            // parse expression and set m_address
            shouldBreak = true;
        } break;
        default: {
            auto maybeMnemonic = find_mnemonic(token);
            if (maybeMnemonic.has_value()) { // mnemonic
                u16 size = maybeMnemonic->get().size;
                m_address += size;
                line.size = size;
                line.bytes[0] = maybeMnemonic->get().byte;
                switch (maybeMnemonic->get().arg) {
                case Mnemonic::Arg::None:
                    line.isComplete = true;
                    break;
                case Mnemonic::Arg::Immediate4: break;
                case Mnemonic::Arg::Immediate12: break;
                case Mnemonic::Arg::Register: break;
                case Mnemonic::Arg::RegisterPair: break;
                case Mnemonic::Arg::RegisterPairImmediate8: break;
                case Mnemonic::Arg::ConditionImmediate8: break;
                }

                pos = str.find_first_not_of(' ', pos);
                if (pos != std::string_view::npos) {
                    line.argStr = str.substr(pos);
                }

                shouldBreak = true;
            }
            else { // SYM label for equate
                std::cout << "test";
                size_t equal = str.find_first_of('=');
                if (equal == std::string_view::npos) {
                    // error!
                    assert(false);
                }
                size_t expStart = str.find_first_not_of(' ', equal + 1);
                std::string_view expStr = str.substr(expStart);
                u16 value = parseExpression(expStr);
                m_labels.emplace(token, value);
            }
        } break;
        }

        if (shouldBreak) {
            break;
        }

        pos = str.find_first_not_of(' ', pos);
        if (pos != std::string_view::npos) {
            str = str.substr(pos);
        }
    }
}

void Assembler40xx::parseLine2ndPass(Line& line) {
    std::string_view str = line.argStr;
    size_t blank = str.find_first_of(' ');
    str = (blank != std::string_view::npos) ? str.substr(0, blank) : str;
    [[maybe_unused]]u16 value = parseExpression(str);
}

// TODO(Kostu): move to cpp only?
u16 Assembler40xx::parseExpression(std::string_view str)
{
    size_t op = str.find_first_of("+-");
    std::string_view token = (op != std::string_view::npos) ? str.substr(0, op) : str;

    u16 value = 0;
    if (std::isdigit(token.front())) {
        int base = 10;
        if (token.starts_with("0x") || token.starts_with("0X10")) {
            base = 16;
            token.remove_prefix(2);
        }
        else if (token.starts_with("0b") || token.starts_with("0B")) {
            base = 2;
            token.remove_prefix(2);
        }

        auto [ptr, ec] = std::from_chars(token.data(), token.data() + token.size(), value, base);
        if (ec != std::errc{}) {
            // error!
            assert(false);
            return 0;
        }
    }
    else {
        auto it = m_labels.find(token);
        if (it == m_labels.end()) {
            // error!
        }
        value = it->second;
    }
    
    return value;
}

constexpr std::optional<std::reference_wrapper<const Assembler40xx::Mnemonic>>
Assembler40xx::find_mnemonic(std::string_view key) {
    for (auto& e : s_mnemonics) {
        if (e.key == key) {
            return std::cref(e);
        }
    }
    return std::nullopt;
}

namespace ASM40xx {

#define PRINT1 printBytes(ss, code, addr, 1, &byte)
#define INST1(str) ss << "    " str
#define INSTR(str, reg) ss << "    " str " " << (reg)
#define INST2(str) do { PRINT1; ss << " " str << std::setw(2) << to_u16(byte); } while(false)
#define INST3(str, x) do { PRINT1; ss << " " str " " << to_u16(x) << ", " << std::setw(2) << to_u16(byte); } while(false)

void disassemble(const u8* code, size_t code_size, std::vector<DisassemblyLine>& output) {
        for (size_t addr = 0; addr < code_size; ) {
            output.emplace_back();
            DisassemblyLine& line = output.back();

            line.address = to_u16(addr);
            std::stringstream ss;
            ss << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << addr << ":  ";
            u8 opcode = code[addr++];
            ss << std::hex << std::setw(2) << to_u16(opcode) << ' ';

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
            case 0x4: PRINT1; ss << " JUN " << std::setw(3) << ((to_u16(opcode) & 0xF) << 8 | byte); break;
            case 0x5: PRINT1; ss << " JMS " << std::setw(3) << ((to_u16(opcode) & 0xF) << 8 | byte); break;
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

            line.str = std::move(ss).str();
        }
    }

} // namespace ASM40xx
