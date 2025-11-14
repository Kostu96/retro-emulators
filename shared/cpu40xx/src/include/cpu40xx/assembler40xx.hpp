#pragma once
#include "utils/asm_common.hpp"

#include <array>
#include <optional>
#include <span>
#include <sstream>
#include <string_view>
#include <unordered_map>
#include <vector>

struct DisassemblyLine;

class Assembler40xx :
    NonCopyable {
public:
    enum class Status { Success, Error };

    explicit Assembler40xx(std::string_view source);

    Status assemble();
    std::string_view getHex() const { return m_hex; }
    std::string_view getLog() const { return m_log; }
private:
    struct Mnemonic {
        enum class Arg : u8 {
            None,
            Register,
            RegisterPair,
            Immediate4,
            Immediate12,
            RegisterImmediate8,
            RegisterPairImmediate8,
            ConditionImmediate8
        };

        std::string_view key{};
        u8 size = 0;
        u8 byte = 0;
        Arg arg = Arg::None;
    };

    struct Line {
        std::optional<Mnemonic> mnemonic{};
        std::string_view str{};
        std::string_view argStr{};
        u32 lineNumber = 0;
        u16 address = 0;
        u8 bytes[2]{};
    };

    void parseLine1stPass(Line& line);
    void parseLine2ndPass(Line& line);
    u16 parseExpression(std::string_view str);

    std::string_view m_source;
    std::vector<Line> m_lines;
    std::unordered_map<std::string_view, u16> m_labels;
    std::string m_hex;
    std::ostringstream m_logStream;
    std::string m_log;
    u16 m_address{};
    bool m_hasError = false;

    static constexpr std::array<Mnemonic, 46> s_mnemonics = {
        Mnemonic{ "ADD", 1, 0x80, Mnemonic::Arg::Register },
        Mnemonic{ "ADM", 1, 0xEB, Mnemonic::Arg::None },
        Mnemonic{ "BBL", 1, 0xC0, Mnemonic::Arg::Immediate4 },
        Mnemonic{ "CLB", 1, 0xF0, Mnemonic::Arg::None },
        Mnemonic{ "CLC", 1, 0xF1, Mnemonic::Arg::None },
        Mnemonic{ "CMA", 1, 0xF4, Mnemonic::Arg::None },
        Mnemonic{ "CMC", 1, 0xF3, Mnemonic::Arg::None },
        Mnemonic{ "DAA", 1, 0xFB, Mnemonic::Arg::None },
        Mnemonic{ "DAC", 1, 0xF8, Mnemonic::Arg::None },
        Mnemonic{ "DCL", 1, 0xFD, Mnemonic::Arg::None },
        Mnemonic{ "FIM", 2, 0x20, Mnemonic::Arg::RegisterPairImmediate8 },
        Mnemonic{ "FIN", 1, 0x30, Mnemonic::Arg::RegisterPair },
        Mnemonic{ "IAC", 1, 0xF2, Mnemonic::Arg::None },
        Mnemonic{ "INC", 1, 0x60, Mnemonic::Arg::Register },
        Mnemonic{ "ISZ", 2, 0x70, Mnemonic::Arg::RegisterPairImmediate8 },
        Mnemonic{ "JCN", 2, 0x10, Mnemonic::Arg::ConditionImmediate8 },
        Mnemonic{ "JIN", 1, 0x31, Mnemonic::Arg::RegisterPair },
        Mnemonic{ "JMS", 2, 0x50, Mnemonic::Arg::Immediate12 },
        Mnemonic{ "JUN", 2, 0x40, Mnemonic::Arg::Immediate12 },
        Mnemonic{ "KBP", 1, 0xFC, Mnemonic::Arg::None },
        Mnemonic{ "LD",  1, 0xA0, Mnemonic::Arg::Register },
        Mnemonic{ "LDM", 1, 0xD0, Mnemonic::Arg::Immediate4 },
        Mnemonic{ "NOP", 1, 0x00, Mnemonic::Arg::None },
        Mnemonic{ "RAL", 1, 0xF5, Mnemonic::Arg::None },
        Mnemonic{ "RAR", 1, 0xF6, Mnemonic::Arg::None },
        Mnemonic{ "RD0", 1, 0xEC, Mnemonic::Arg::None },
        Mnemonic{ "RD1", 1, 0xED, Mnemonic::Arg::None },
        Mnemonic{ "RD2", 1, 0xEE, Mnemonic::Arg::None },
        Mnemonic{ "RD3", 1, 0xEF, Mnemonic::Arg::None },
        Mnemonic{ "RDM", 1, 0xE9, Mnemonic::Arg::None },
        Mnemonic{ "RDR", 1, 0xEA, Mnemonic::Arg::None },
        Mnemonic{ "SBM", 1, 0xE8, Mnemonic::Arg::None },
        Mnemonic{ "SRC", 1, 0x21, Mnemonic::Arg::RegisterPair },
        Mnemonic{ "STC", 1, 0xFA, Mnemonic::Arg::None },
        Mnemonic{ "SUB", 1, 0x90, Mnemonic::Arg::Register },
        Mnemonic{ "TCC", 1, 0xF7, Mnemonic::Arg::None },
        Mnemonic{ "TCS", 1, 0xF9, Mnemonic::Arg::None },
        Mnemonic{ "WMP", 1, 0xE1, Mnemonic::Arg::None },
        Mnemonic{ "WPM", 1, 0xE3, Mnemonic::Arg::None },
        Mnemonic{ "WR0", 1, 0xE4, Mnemonic::Arg::None },
        Mnemonic{ "WR1", 1, 0xE5, Mnemonic::Arg::None },
        Mnemonic{ "WR2", 1, 0xE6, Mnemonic::Arg::None },
        Mnemonic{ "WR3", 1, 0xE7, Mnemonic::Arg::None },
        Mnemonic{ "WRM", 1, 0xE0, Mnemonic::Arg::None },
        Mnemonic{ "WRR", 1, 0xE2, Mnemonic::Arg::None },
        Mnemonic{ "XCH", 1, 0xB0, Mnemonic::Arg::Register }
    };

    static constexpr std::optional<std::reference_wrapper<const Mnemonic>> find_mnemonic(std::string_view key);
};

namespace ASM40xx
{
std::vector<DisassemblyLine> disassemble(std::span<const u8> code);
}
