#include "cpu40xx/assembler40xx.hpp"
#include "instruction_masks.hpp"
#include "utils/disassembly_line.hpp"

#include <cassert>
#include <charconv>
#include <format>

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
            if (firstNonBlank != std::string_view::npos) {
                size_t lastNonBlank = line.find_last_not_of(' ');
                m_lines.emplace_back(Line{
                    .str = line.substr(firstNonBlank, lastNonBlank - firstNonBlank + 1),
                    .lineNumber = lineNumber
                });
            }
        }

        start = endLine + 1;
        lineNumber++;
    }

    m_address = 0;

    if (m_lines.empty()) {
        m_logStream << "Warning[0]: source is empty.\n";
    }

    for (auto& l : m_lines) {
        parseLine1stPass(l);
    }

    for (auto& l : m_lines) {
        if (l.type == Line::Type::Instruction) {
            parseLine2ndPass(l);
        }
    }

    std::ostringstream hexStream;
    std::vector<std::pair<std::streampos, u8>> sizes;
    u8 size = 0;
    u8 checksum = 0;
    for (auto& l : m_lines) {
        if (size == 0) {
            hexStream << ':';                              // start code
            sizes.emplace_back(std::make_pair(hexStream.tellp(), to_u8(0)));
            hexStream << "??";                             // byte count placeholder
            hexStream << std::format("{:04X}", l.address); // address
            hexStream << "00";                             // record type
            checksum += l.address & 0xFF;
            checksum += (l.address >> 8) & 0xFF;
        }
        size += l.type == Line::Type::Instruction ? l.instruction.mnemonic->size : 1;
        hexStream << std::format("{:02X}", l.bytes[0]); // data byte 0
        checksum += l.bytes[0];
        if (l.type == Line::Type::Instruction && l.instruction.mnemonic->size > 1) {
            hexStream << std::format("{:02X}", l.bytes[1]); // data byte 1
            checksum += l.bytes[1];
        }
        if (size >= 0x08) {
            checksum += size;
            sizes.back().second = size;
            size = 0;
            checksum = ~checksum + 1;
            hexStream << std::format("{:02X}", checksum); // checksum
            hexStream << '\n';
            checksum = 0;
        }
    }
    if (size != 0) {
        checksum += size;
        sizes.back().second = size;
        size = 0;
        checksum = ~checksum + 1;
        hexStream << std::format("{:02X}", checksum); // checksum
        hexStream << '\n';
        checksum = 0;
    }
    hexStream << ":00000001FF";
    m_hex = hexStream.str();

    for (auto& s : sizes) {
        auto sizeStr = std::format("{:02X}", s.second);
        m_hex.replace(static_cast<size_t>(s.first), 2, sizeStr);
    }

    m_log = m_logStream.str();

    return m_hasError ? Status::Error : Status::Success;
}

void Assembler40xx::parseLine1stPass(Line& line) {
    line.address = m_address;
    std::string_view str = line.str;
    bool shouldBreak = false;
    while (true) {
        size_t pos = str.find_first_of(' ');
        std::string_view token = str.substr(0, pos);

        char last = token.back();
        switch (last) {
        case ',': { // Label
            m_labels.emplace(token.substr(0, token.size() - 1), m_address);
        } break;
        case '=': {
            // parse expression and set m_address
            assert(false);
            shouldBreak = true;
        } break;
        default: {
            auto maybeMnemonic = find_mnemonic(token);
            if (maybeMnemonic.has_value()) { // mnemonic
                line.instruction.mnemonic = maybeMnemonic->get();
                m_address += line.instruction.mnemonic->size;

                pos = str.find_first_not_of(' ', pos);
                if (pos != std::string_view::npos) {
                    line.instruction.argStr = str.substr(pos);
                }

                shouldBreak = true;
            }
            else { // SYM label for equate or data statement
                size_t equal = str.find_first_of('=');
                if (equal == std::string_view::npos) {
                    // data statement
                    m_address += 1;
                    u16 value = parseExpression(str, line.address);
                    line.type = Line::Type::Data;
                    line.bytes[0] = value & 0xFF; // TODO(Kostu): truncation warning
                    shouldBreak = true;
                }
                else {
                    size_t expStart = str.find_first_not_of(' ', equal + 1);
                    std::string_view expStr = str.substr(expStart);
                    u16 value = parseExpression(expStr, line.address);
                    m_labels.emplace(token, value);
                }
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
    assert(line.type == Line::Type::Instruction);

    size_t blank = line.instruction.argStr.find_first_of(' ');
    bool hasTwoExpressions = blank != std::string_view::npos;
    std::string_view str1 = (hasTwoExpressions) ? line.instruction.argStr.substr(0, blank) : line.instruction.argStr;
    std::string_view str2 = (hasTwoExpressions) ? line.instruction.argStr.substr(blank + 1) : "";

    switch (line.instruction.mnemonic->arg) {
    case Mnemonic::Arg::None:
        if (!str1.empty() || !str2.empty()) {
            // error! unexpeted argument
            m_hasError = true;
            assert(false);
        }
        break;
    case Mnemonic::Arg::Register:
    case Mnemonic::Arg::RegisterPair:
    case Mnemonic::Arg::Immediate4:
    case Mnemonic::Arg::Immediate12:
        if (str1.empty()) {
            // error! expected argument
            m_hasError = true;
            assert(false);
        }
        if (!str2.empty()) {
            // error! unexpected argument
            m_hasError = true;
            assert(false);
        }
        break;
    case Mnemonic::Arg::RegisterImmediate8:
    case Mnemonic::Arg::RegisterPairImmediate8:
    case Mnemonic::Arg::ConditionImmediate8:
        if (str1.empty()) {
            // error! expected argument
            m_hasError = true;
            assert(false);
        }
        if (str2.empty()) {
            // error! expected argument
            m_hasError = true;
            assert(false);
        }
        break;
    }

    if (m_hasError) return;

    switch (line.instruction.mnemonic->arg) {
    case Mnemonic::Arg::None:
        line.bytes[0] = line.instruction.mnemonic->byte;
        break;
    case Mnemonic::Arg::Immediate4:
    case Mnemonic::Arg::Register: {
        u16 value = parseExpression(str1, line.address);
        if ((value >> 4) > 0) {
            m_logStream << "Warning[" << line.lineNumber << "]: expression truncated to fit argument.\n";
        }
        line.bytes[0] = line.instruction.mnemonic->byte | (value & 0xF);
    } break;
    case Mnemonic::Arg::Immediate12: {
        u16 value = parseExpression(str1, line.address);
        line.bytes[0] = line.instruction.mnemonic->byte | (value >> 8);
        line.bytes[1] = value & 0xFF;
    } break;
    case Mnemonic::Arg::RegisterPair: {
        u16 value = parseExpression(str1, line.address);
        if ((value >> 4) > 0) {
            m_logStream << "Warning[" << line.lineNumber << "]: expression truncated to fit argument.\n";
        }
        if (value & 1) {
            m_logStream << "Error[" << line.lineNumber << "]: invalid register pair index!\n";
            m_hasError = true;
        }
        line.bytes[0] = line.instruction.mnemonic->byte | (value & 0xF);
    } break;
    case Mnemonic::Arg::RegisterImmediate8:
    case Mnemonic::Arg::RegisterPairImmediate8:
    case Mnemonic::Arg::ConditionImmediate8: {
        u16 value1 = parseExpression(str1, line.address);
        u16 value2 = parseExpression(str2, line.address);
        line.bytes[0] = line.instruction.mnemonic->byte | (value1 & 0xF);
        line.bytes[1] = value2 & 0xFF;
    } break;
    }
}

// TODO(Kostu): move to cpp only?
u16 Assembler40xx::parseExpression(std::string_view str, u16 current_address) {
    size_t op = str.find_first_of("+-", 1);
    std::string_view token = (op != std::string_view::npos) ? str.substr(0, op) : str;
    str = (op != std::string_view::npos) ? str.substr(op) : "";

    u16 value = 0;
    while (true) {
        bool subtract = token.front() == '-';
        if (token.front() == '-' || token.front() == '+') {
            token.remove_prefix(1);
        }

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
            else if (token.front() == '0') {
                base = 8;
            }

            u16 parsedValue;
            auto [ptr, ec] = std::from_chars(token.data(), token.data() + token.size(), parsedValue, base);
            if (ec != std::errc{}) {
                // error!
                assert(false);
                return 0;
            }
            value += subtract ? -parsedValue : parsedValue;
        }
        else {
            auto it = m_labels.find(token);
            if (it == m_labels.end()) {
                if (token == "*") {
                    value += subtract ? -current_address : current_address;
                }
                else {
                    // error!
                    assert(false);
                }
            }
            else {
                value += subtract ? -it->second : it->second;
            }
        }

        if (op != std::string_view::npos) {
            op = str.find_first_of("+-", 1);
            token = (op != std::string_view::npos) ? str.substr(0, op) : str;
        }
        else {
            break;
        }
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

#define PRINT1 printBytes(ss, code.data(), addr, 1, &byte)
#define INSTR(str, reg) ss << "    " str " " << (reg)
#define INST2(str) do { PRINT1; ss << " " str << std::setw(2) << to_u16(byte); } while(false)

static inline void printNoArgInst(std::ostringstream& ss, std::string_view str) {
    ss << "    " << str;
}

static inline void print2ArgInst(std::ostringstream& ss, std::string_view str, u8 arg1) {
    ss << std::format("    {} {:02X}", str, arg1);
}

// TODO(Kostu): Make this right
std::vector<DisassemblyLine> disassemble(std::span<const u8> code) {
    std::vector<DisassemblyLine> output;
    for (size_t addr = 0; addr < code.size(); ) {
        output.emplace_back();
        DisassemblyLine& line = output.back();

        line.address = to_u16(addr);
        std::ostringstream ss;
        ss << std::format("{:04X}:  ", addr);
        u8 opcode = code[addr++];
        ss << std::format("{:02X} ", opcode);

        u8 byte;
        switch (opcode >> 4)
        {
        case CPU40xxInstructionHighNibbleMasks::NOP:
            if (opcode & 0xF)
                printNoArgInst(ss, "???");
            else
                printNoArgInst(ss, "NOP");
            break;
        case CPU40xxInstructionHighNibbleMasks::JCN: print2ArgInst(ss, "JCN", opcode & 0xF); break;
        case CPU40xxInstructionHighNibbleMasks::FIMorSRC:
            if (opcode & 1)
                INSTR("SRC", opcode & 0xE);
            else
                print2ArgInst(ss, "FIM", opcode & 0xE);
            break;
        case CPU40xxInstructionHighNibbleMasks::FINorJIN:
            if (opcode & 1)
                INSTR("JIN", opcode & 0xE);
            else
                INSTR("FIN", opcode & 0xE);
            break;
        case CPU40xxInstructionHighNibbleMasks::JUN: PRINT1; ss << std::format(" JUN {:03X}", to_u16(opcode & 0xF) << 8 | byte); break;
        case CPU40xxInstructionHighNibbleMasks::JMS: PRINT1; ss << std::format(" JMS {:03X}", to_u16(opcode & 0xF) << 8 | byte); break;
        case CPU40xxInstructionHighNibbleMasks::INC: INSTR("INC", opcode & 0xF); break;
        case CPU40xxInstructionHighNibbleMasks::ISZ: print2ArgInst(ss, "ISZ", opcode & 0xF); break;
        case CPU40xxInstructionHighNibbleMasks::ADD: INSTR("ADD", opcode & 0xF); break;
        case CPU40xxInstructionHighNibbleMasks::SUB: INSTR("SUB", opcode & 0xF); break;
        case CPU40xxInstructionHighNibbleMasks::LD: INSTR("LD ", opcode & 0xF); break;
        case CPU40xxInstructionHighNibbleMasks::XCH: INSTR("XCH", opcode & 0xF); break;
        case CPU40xxInstructionHighNibbleMasks::BBL: INSTR("BBL", opcode & 0xF); break;
        case CPU40xxInstructionHighNibbleMasks::LDM: INSTR("LDM", opcode & 0xF); break;
        case CPU40xxInstructionHighNibbleMasks::MemoryGroup:
            switch (opcode & 0xF)
            {
            case CPU40xxInstructionLowNibbleMasks::WRM: printNoArgInst(ss, "WRM"); break;
            case CPU40xxInstructionLowNibbleMasks::WMP: printNoArgInst(ss, "WMP"); break;
            case CPU40xxInstructionLowNibbleMasks::WRR: printNoArgInst(ss, "WRR"); break;
            case CPU40xxInstructionLowNibbleMasks::WPM: printNoArgInst(ss, "WPM"); break;
            case CPU40xxInstructionLowNibbleMasks::WR0: printNoArgInst(ss, "WR0"); break;
            case CPU40xxInstructionLowNibbleMasks::WR1: printNoArgInst(ss, "WR1"); break;
            case CPU40xxInstructionLowNibbleMasks::WR2: printNoArgInst(ss, "WR2"); break;
            case CPU40xxInstructionLowNibbleMasks::WR3: printNoArgInst(ss, "WR3"); break;
            case CPU40xxInstructionLowNibbleMasks::SBM: printNoArgInst(ss, "SBM"); break;
            case CPU40xxInstructionLowNibbleMasks::RDM: printNoArgInst(ss, "RDM"); break;
            case CPU40xxInstructionLowNibbleMasks::RDR: printNoArgInst(ss, "RDR"); break;
            case CPU40xxInstructionLowNibbleMasks::ADM: printNoArgInst(ss, "ADM"); break;
            case CPU40xxInstructionLowNibbleMasks::RD0: printNoArgInst(ss, "RD0"); break;
            case CPU40xxInstructionLowNibbleMasks::RD1: printNoArgInst(ss, "RD1"); break;
            case CPU40xxInstructionLowNibbleMasks::RD2: printNoArgInst(ss, "RD2"); break;
            case CPU40xxInstructionLowNibbleMasks::RD3: printNoArgInst(ss, "RD3"); break;
            default: printNoArgInst(ss, "???");
            }
            break;
        case CPU40xxInstructionHighNibbleMasks::AccumulatorGroup:
            switch (opcode & 0xF)
            {
            case CPU40xxInstructionLowNibbleMasks::CLB: printNoArgInst(ss, "CLB"); break;
            case CPU40xxInstructionLowNibbleMasks::CLC: printNoArgInst(ss, "CLC"); break;
            case CPU40xxInstructionLowNibbleMasks::IAC: printNoArgInst(ss, "IAC"); break;
            case CPU40xxInstructionLowNibbleMasks::CMC: printNoArgInst(ss, "CMC"); break;
            case CPU40xxInstructionLowNibbleMasks::CMA: printNoArgInst(ss, "CMA"); break;
            case CPU40xxInstructionLowNibbleMasks::RAL: printNoArgInst(ss, "RAL"); break;
            case CPU40xxInstructionLowNibbleMasks::RAR: printNoArgInst(ss, "RAR"); break;
            case CPU40xxInstructionLowNibbleMasks::TCC: printNoArgInst(ss, "TCC"); break;
            case CPU40xxInstructionLowNibbleMasks::DAC: printNoArgInst(ss, "DAC"); break;
            case CPU40xxInstructionLowNibbleMasks::TCS: printNoArgInst(ss, "TCS"); break;
            case CPU40xxInstructionLowNibbleMasks::STC: printNoArgInst(ss, "STC"); break;
            case CPU40xxInstructionLowNibbleMasks::DAA: printNoArgInst(ss, "DAA"); break;
            case CPU40xxInstructionLowNibbleMasks::KBP: printNoArgInst(ss, "KBP"); break;
            case CPU40xxInstructionLowNibbleMasks::DCL: printNoArgInst(ss, "DCL"); break;
            default: printNoArgInst(ss, "???");
            }
            break;
        default: printNoArgInst(ss, "???");
        }

        line.str = std::move(ss).str();
    }

    return output;
}

} // namespace ASM40xx
