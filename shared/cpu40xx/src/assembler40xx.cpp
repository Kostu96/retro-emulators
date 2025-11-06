#include "cpu40xx/assembler40xx.hpp"
#include "utils/disassembly_line.hpp"

#include <cassert>
#include <charconv>
#include <format>
#include <iomanip>

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

    if (m_lines.empty()) {
        m_logStream << "Warning[0]: source is empty.";
    }

    for (auto& l : m_lines) {
        parseLine1stPass(l);
    }

    // Remove lines without instructions (origin and equate)
    std::erase_if(m_lines, [](auto& e) { return !e.mnemonic.has_value(); });

    for (auto& l : m_lines) {
        parseLine2ndPass(l);
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
        size += l.mnemonic->size;
        hexStream << std::format("{:02X}", l.bytes[0]); // data byte 0
        checksum += l.bytes[0];
        if (l.mnemonic->size > 1) {
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
        }
    }
    if (size != 0) {
        checksum += size;
        sizes.back().second = size;
        size = 0;
        checksum = ~checksum + 1;
        hexStream << std::format("{:02X}", checksum); // checksum
        hexStream << '\n';
    }
    hexStream << ":00000001FF";
    m_hex = hexStream.str();

    for (auto& s : sizes) {
        auto sizeStr = std::format("{:02X}", s.second);
        m_hex.replace(static_cast<size_t>(s.first), 2, sizeStr);
    }

    m_log = m_logStream.str();

    return hasError ? Status::Error : Status::Success;
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
                line.mnemonic = maybeMnemonic->get();
                m_address += line.mnemonic->size;

                pos = str.find_first_not_of(' ', pos);
                if (pos != std::string_view::npos) {
                    line.argStr = str.substr(pos);
                }

                shouldBreak = true;
            }
            else { // SYM label for equate
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
    size_t blank = line.argStr.find_first_of(' ');
    bool hasTwoExpressions = blank != std::string_view::npos;
    std::string_view str1 = (hasTwoExpressions) ? line.argStr.substr(0, blank) : line.argStr;
    std::string_view str2 = (hasTwoExpressions) ? line.argStr.substr(blank) : "";

    switch (line.mnemonic->arg) {
    case Mnemonic::Arg::None:
        if (!str1.empty() || !str2.empty()) {
            // error! unexpeted argument
            assert(false);
        }
        line.bytes[0] = line.mnemonic->byte;
        break;
    case Mnemonic::Arg::Immediate4: {
        if (str1.empty()) {
            // error! expected argument
            assert(false);
        }
        if (!str2.empty()) {
            // error! unexpected argument
            assert(false);
        }
        u16 value = parseExpression(str1);
        if ((value >> 4) > 0) {
            // warning. argument truncated
        }
        line.bytes[0] = line.mnemonic->byte | (value & 0xF);
    } break;
    case Mnemonic::Arg::Immediate12:
        assert(false);
        break;
    case Mnemonic::Arg::Register:
        assert(false);
        break;
    case Mnemonic::Arg::RegisterPair:
        assert(false);
        break;
    case Mnemonic::Arg::RegisterPairImmediate8:
        assert(false);
        break;
    case Mnemonic::Arg::ConditionImmediate8:
        assert(false);
        break;
    }
}

// TODO(Kostu): move to cpp only?
u16 Assembler40xx::parseExpression(std::string_view str)
{
    //size_t lastOp = 0;
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
                // error!
                assert(false);
            }
            value += subtract ? -it->second : it->second;
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
