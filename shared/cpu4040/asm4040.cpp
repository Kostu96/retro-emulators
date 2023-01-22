#include "asm4040.hpp"

#include <cassert>
#include <unordered_map>

namespace ASM4040
{
    struct Token
    {
        enum class Type
        {
            MN_ADD, MN_ADM,
            MN_BBL,
            MN_CLB, MN_CLC, MN_CMA, MN_CMC,
            MN_DAA, MN_DAC, MN_DCL,
            MN_FIM, MN_FIN,
            MN_IAC, MN_INC, MN_ISZ,
            MN_JCN, MN_JIN, MN_JMS, MN_JUN,
            MN_KBP,
            MN_LD,  MN_LDM,
            MN_RAL, MN_RAR, MN_RDM, MN_RDR, MN_RDX,
            MN_SBM, MN_SRC, MN_STC, MN_SUB,
            MN_TCC, MN_TCS,
            MN_WMP, MN_WRM, MN_WRR, MN_WRX,
            MN_XCH,
            Label,
            DecimalNumber,
            Plus,
            Minus,
            Star,
            Comma,
            Equal,
            Error,
            EndOfSource
        };

        Type type = Type::Error;
        const char* start = nullptr;
        u32 length = 0;
        u32 line = (u32)-1;
    };

    class Scanner
    {
    public:
        explicit Scanner(const char* source) :
            start{ source },
            current{ source },
            line{ 1 } {}

        Token nextToken()
        {
            skipWhitespace();
            start = current;
            if (*current == '\0') return makeToken(Token::Type::EndOfSource);

            if (isAlpha(*current))
            {
                while (isAlpha(*current) || isDigit(*current)) current++;
                return makeToken(identifierType());
            }

            if (isDigit(*current))
            {
                while (isDigit(*current)) current++;

                return makeToken(Token::Type::DecimalNumber);
            }

            switch (*current++)
            {
            case '+': return makeToken(Token::Type::Plus);
            case '-': return makeToken(Token::Type::Minus);
            case '*': return makeToken(Token::Type::Star);
            case ',': return makeToken(Token::Type::Comma);
            case '=': return makeToken(Token::Type::Equal);
            }

            return makeToken(Token::Type::Error);
        }
    private:
        Token::Type identifierType()
        {
            switch (start[0]) {
            case 'A':
                if (current - start > 1 && start[1] == 'D' && current - start > 2)
                    switch (start[2])
                    {
                    case 'D': return checkMnemonic(3, 0, "", Token::Type::MN_ADD);
                    case 'M': return checkMnemonic(3, 0, "", Token::Type::MN_ADM);
                    }
                break;
            case 'B': return checkMnemonic(1, 2, "BL", Token::Type::MN_BBL);
            case 'C':
                if (current - start > 1)
                    switch (start[1])
                    {
                    case 'L':
                        if (current - start > 2)
                            switch (start[2])
                            {
                            case 'B': return checkMnemonic(3, 0, "", Token::Type::MN_CLB);
                            case 'C': return checkMnemonic(3, 0, "", Token::Type::MN_CLC);
                            }
                        break;
                    case 'M':
                        if (current - start > 2)
                            switch (start[2])
                            {
                            case 'A': return checkMnemonic(3, 0, "", Token::Type::MN_CMA);
                            case 'C': return checkMnemonic(3, 0, "", Token::Type::MN_CMC);
                            }
                        break;
                    }
                break;
            case 'D':
                if (current - start > 1)
                    switch (start[1])
                    {
                    case 'A':
                        if (current - start > 2)
                            switch (start[2])
                            {
                            case 'A': return checkMnemonic(3, 0, "", Token::Type::MN_DAA);
                            case 'C': return checkMnemonic(3, 0, "", Token::Type::MN_DAC);
                            }
                        break;
                    case 'C': return checkMnemonic(1, 2, "CL", Token::Type::MN_DCL);
                    }
                break;
            case 'F':
                if (current - start > 1 && start[1] == 'I' && current - start > 2)
                    switch (start[2])
                    {
                    case 'M': return checkMnemonic(3, 0, "", Token::Type::MN_FIM);
                    case 'N': return checkMnemonic(3, 0, "", Token::Type::MN_FIN);
                    }
                break;
            case 'I':
                if (current - start > 1)
                    switch (start[1])
                    {
                    case 'A': return checkMnemonic(2, 1, "C", Token::Type::MN_IAC);
                    case 'N': return checkMnemonic(2, 1, "C", Token::Type::MN_INC);
                    case 'S': return checkMnemonic(2, 1, "Z", Token::Type::MN_ISZ);
                    }
                break;
            case 'J':
                if (current - start > 1)
                    switch (start[1])
                    {
                    case 'C': return checkMnemonic(2, 1, "N", Token::Type::MN_JCN);
                    case 'I': return checkMnemonic(2, 1, "N", Token::Type::MN_JIN);
                    case 'M': return checkMnemonic(2, 1, "S", Token::Type::MN_JMS);
                    case 'U': return checkMnemonic(2, 1, "N", Token::Type::MN_JUN);
                    }
                break;
            case 'K': return checkMnemonic(1, 2, "BP", Token::Type::MN_KBP);
            case 'L':
                if (current - start > 1 && start[1] == 'D')
                {
                    if (current - start > 2 && start[2] == 'M')
                        return checkMnemonic(3, 0, "", Token::Type::MN_LDM);

                    return checkMnemonic(2, 0, "", Token::Type::MN_LD);
                }
                break;
            case 'R':
                if (current - start > 1)
                    switch (start[1])
                    {
                    case 'A':
                        if (current - start > 2)
                            switch (start[2])
                            {
                            case 'L': return checkMnemonic(3, 0, "", Token::Type::MN_RAL);
                            case 'R': return checkMnemonic(3, 0, "", Token::Type::MN_RAR);
                            }
                        break;
                    case 'D':
                        if (current - start > 2)
                            switch (start[2])
                            {
                            case 'M': return checkMnemonic(3, 0, "", Token::Type::MN_RDM);
                            case 'R': return checkMnemonic(3, 0, "", Token::Type::MN_RDR);
                            case 'X': return checkMnemonic(3, 0, "", Token::Type::MN_RDX);
                            }
                        break;
                    }
                break;
            case 'S':
                if (current - start > 1)
                    switch (start[1])
                    {
                    case 'B': return checkMnemonic(2, 1, "M", Token::Type::MN_SBM);
                    case 'R': return checkMnemonic(2, 1, "C", Token::Type::MN_SRC);
                    case 'T': return checkMnemonic(2, 1, "C", Token::Type::MN_STC);
                    case 'U': return checkMnemonic(2, 1, "B", Token::Type::MN_SUB);
                    }
                break;
            case 'T':
                if (current - start > 1 && start[1] == 'C' && current - start > 2)
                    switch (start[2])
                    {
                    case 'C': return checkMnemonic(3, 0, "", Token::Type::MN_TCC);
                    case 'S': return checkMnemonic(3, 0, "", Token::Type::MN_TCS);
                    }
                break;
            case 'W':
                if (current - start > 1)
                    switch (start[1])
                    {
                    case 'M': return checkMnemonic(2, 1, "P", Token::Type::MN_WMP);
                    case 'R':
                        switch (start[2])
                        {
                        case 'M': return checkMnemonic(3, 0, "", Token::Type::MN_WRM);
                        case 'R': return checkMnemonic(3, 0, "", Token::Type::MN_WRR);
                        case 'X': return checkMnemonic(3, 0, "", Token::Type::MN_WRX);
                        }
                        break;
                    }
                break;
            case 'X': return checkMnemonic(1, 2, "CH", Token::Type::MN_XCH);
            }

            return Token::Type::Label;
        }

        Token::Type checkMnemonic(int start, int length, const char* rest, Token::Type type)
        {
            if (current - this->start == start + length && memcmp(this->start + start, rest, length) == 0) return type;
            return Token::Type::Label;
        }

        Token makeToken(Token::Type type)
        {
            Token token;
            token.type = type;
            token.start = start;
            token.length = current - start;
            token.line = line;
            return token;
        }

        void skipWhitespace()
        {
            for (;;)
                switch (*current)
                {
                case ' ':
                case '\r':
                case '\t':
                    current++;
                    break;
                case '\n':
                    line++;
                    current++;
                    break;
                case '/':
                    while (*current != '\n' && *current != '\0')
                        current++;
                    break;
                default:
                    return;
                }
        }

        const char* start;
        const char* current;
        u32 line;
    };

    void assemble(const char* source, std::vector<u8>& output)
    {
        std::unordered_map<std::string_view, u16> labels;
        Scanner scanner{ source };

        Token previous{};
        Token current{};
        u32 line = -1;
        while (true)
        {
            previous = current;
            current = scanner.nextToken();

            if (previous.type == Token::Type::Label)
            {
                std::string_view str{ previous.start, previous.length };
                if (current.type == Token::Type::Comma)// || current.type == Token::Type::Equal)
                    if (!labels.contains(str))
                    {
                        u16 value = 0;
                        labels.emplace(str, value);
                    }
                    else
                    {
                        // error, multiple label or define
                    }
            }

            if (current.line != line) {
                printf("%4d ", current.line);
                line = current.line;
            }
            else {
                printf("   | ");
            }
            printf("%2d '%.*s'\n", current.type, current.length, current.start);

            if (current.type == Token::Type::EndOfSource) break;
        }
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
