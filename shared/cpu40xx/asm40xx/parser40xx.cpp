#include "parser40xx.hpp"

#include <cstdio>

namespace ASM40xx {

    Parser::Parser(const char* source) :
        m_scanner{ source },
        m_address{ 0 },
        m_hadError{ false },
        m_panicMode{ false },
        m_line{ 1 }
    {
        m_symbolTable.reserve(40);
        m_symbolTable.emplace(std::string_view{ "P0" }, 0x0);
        m_symbolTable.emplace(std::string_view{ "P1" }, 0x2);
        m_symbolTable.emplace(std::string_view{ "P2" }, 0x4);
        m_symbolTable.emplace(std::string_view{ "P3" }, 0x6);
        m_symbolTable.emplace(std::string_view{ "P4" }, 0x8);
        m_symbolTable.emplace(std::string_view{ "P5" }, 0xA);
        m_symbolTable.emplace(std::string_view{ "P6" }, 0xC);
        m_symbolTable.emplace(std::string_view{ "P7" }, 0xE);

        m_symbolTable.emplace(std::string_view{ "R0" }, 0x0);
        m_symbolTable.emplace(std::string_view{ "R1" }, 0x1);
        m_symbolTable.emplace(std::string_view{ "R2" }, 0x2);
        m_symbolTable.emplace(std::string_view{ "R3" }, 0x3);
        m_symbolTable.emplace(std::string_view{ "R4" }, 0x4);
        m_symbolTable.emplace(std::string_view{ "R5" }, 0x5);
        m_symbolTable.emplace(std::string_view{ "R6" }, 0x6);
        m_symbolTable.emplace(std::string_view{ "R7" }, 0x7);
        m_symbolTable.emplace(std::string_view{ "R8" }, 0x8);
        m_symbolTable.emplace(std::string_view{ "R9" }, 0x9);

        m_symbolTable.emplace(std::string_view{ "RA" }, 0xA);
        m_symbolTable.emplace(std::string_view{ "RB" }, 0xB);
        m_symbolTable.emplace(std::string_view{ "RC" }, 0xC);
        m_symbolTable.emplace(std::string_view{ "RD" }, 0xD);
        m_symbolTable.emplace(std::string_view{ "RE" }, 0xE);
        m_symbolTable.emplace(std::string_view{ "RF" }, 0xF);

        m_symbolTable.emplace(std::string_view{ "R10" }, 0xA);
        m_symbolTable.emplace(std::string_view{ "R11" }, 0xB);
        m_symbolTable.emplace(std::string_view{ "R12" }, 0xC);
        m_symbolTable.emplace(std::string_view{ "R13" }, 0xD);
        m_symbolTable.emplace(std::string_view{ "R14" }, 0xE);
        m_symbolTable.emplace(std::string_view{ "R15" }, 0xF);
    }

    void Parser::advance()
    {
        m_previous = m_current;

        while (true)
        {
            m_current = m_scanner.getToken();
            if (m_current.type != Token::Type::Error) break;

            errorAt(m_current, m_current.start);
        }
    }

    void Parser::errorAt(const Token& token, const char* message)
    {
        if (m_panicMode) return;
        m_panicMode = true;
        m_hadError = true;

        fprintf(stderr, "Error: %s\n", message);
    }

    void Parser::line()
    {
        if (m_current.type < Token::Type::MNEMONIC_COUNT)
            instruction();
        else if (match(Token::Type::Label))
        {
            std::string_view label{ m_previous.start, m_previous.length };
            if (match(Token::Type::Comma))
            {
                m_symbolTable.emplace(label, m_address);

                if (m_current.type < Token::Type::MNEMONIC_COUNT)
                    instruction();
            }
            else if (match(Token::Type::Equal))
            {
                advance();
                expression();
            }
            else
            {
                errorAt(m_previous, "Unterminated label.");
            }
        }
        else if (match(Token::Type::Equal))
        {
            advance();
            expression();
        }

        consume(Token::Type::EndOfLine, "Expect EOL.");
    }

    void Parser::consume(Token::Type type, const char* message)
    {
        if (m_current.type == type)
        {
            advance();
            return;
        }

        errorAt(m_current, message);
    }

    void Parser::instruction()
    {
        advance();

        switch (m_previous.type)
        {
        case Token::Type::MN_ADD: break;
        case Token::Type::MN_ADM: break;
        case Token::Type::MN_BBL: break;
        case Token::Type::MN_CLB: break;
        case Token::Type::MN_CLC: break;
        case Token::Type::MN_CMA: break;
        case Token::Type::MN_CMC: break;
        case Token::Type::MN_DAA: break;
        case Token::Type::MN_DAC: break;
        case Token::Type::MN_DCL: break;
        case Token::Type::MN_FIM: {
            u16 reg = expression();
            if (reg > 0xF && !(reg & 1))
                errorAt(m_current, "Expression does not evaluate to register pair index.");

            u16 value = expression();
            if (value > 0xFF)
                errorAt(m_current, "Expression overflowed."); // TODO: change to warning

            m_output.push_back(0x20 | reg);
            m_output.push_back(value & 0xFF);
            m_address += 2;
        } break;
        case Token::Type::MN_FIN: break;
        case Token::Type::MN_IAC: break;
        case Token::Type::MN_INC: break;
        case Token::Type::MN_ISZ: break;
        case Token::Type::MN_JCN: break;
        case Token::Type::MN_JIN: break;
        case Token::Type::MN_JMS: break;
        case Token::Type::MN_JUN: {
            u16 value = expression();
            if (value > 0xFFF)
                errorAt(m_current, "Expression overflowed."); // TODO: change to warning

            m_output.push_back(0x40 | (value >> 8));
            m_output.push_back(value & 0xFF);
            m_address += 2;
        } break;
        case Token::Type::MN_KBP: break;
        case Token::Type::MN_LD:  break;
        case Token::Type::MN_LDM: break;
        case Token::Type::MN_NOP:
            m_output.push_back(0x00);
            m_address++;
            break;
        case Token::Type::MN_RAL: break;
        case Token::Type::MN_RAR:
            
            break;
        case Token::Type::MN_RDM: break;
        case Token::Type::MN_RDR:
            m_output.push_back(0xEA);
            m_address++;
            break;
        case Token::Type::MN_RDX: break;
        case Token::Type::MN_SBM: break;
        case Token::Type::MN_SRC: {
            u16 reg = expression();
            if (reg > 0xF && !(reg & 1))
                errorAt(m_current, "Expression does not evaluate to register pair index.");

            m_output.push_back(0x21 | reg);
            m_address++;
        } break;
        case Token::Type::MN_STC: break;
        case Token::Type::MN_SUB: break;
        case Token::Type::MN_TCC: break;
        case Token::Type::MN_TCS: break;
        case Token::Type::MN_WMP: break;
        case Token::Type::MN_WRM: break;
        case Token::Type::MN_WRR: break;
        case Token::Type::MN_WRX: break;
        case Token::Type::MN_XCH: {
            u16 reg = expression();
            if (reg > 0xF)
                errorAt(m_current, "Expression does not evaluate to register index.");

            m_output.push_back(0xB0 | reg);
            m_address++;
        } break;
        }
    }

    u16 Parser::expression()
    {
        if (match(Token::Type::Label))
        {
            std::string_view str{ m_previous.start, m_previous.length };
            if (m_symbolTable.contains(str))
            {
                return m_symbolTable.at(str);
            }
            else
            {

            }
        }
        else if (match(Token::Type::Number))
        {
            return m_previous.value;
        }

        return 0;
    }

    bool Parser::match(Token::Type type)
    {
        if (m_current.type != type) return false;
        advance();
        return true;
    }

}
