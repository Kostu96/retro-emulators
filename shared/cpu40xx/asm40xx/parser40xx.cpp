#include "parser40xx.hpp"

#include <cstdio>

namespace ASM40xx {

    Parser::Parser(const char* source) :
        m_scanner{ source },
        m_address{ 0 },
        m_hadError{ false },
        m_panicMode{ false },
        m_line{ 1 } {}

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

    u8 Parser::mnemonicTypeToByte(Token::Type type) const
    {
        switch (type)
        {
        case Token::Type::MN_ADD: return 0x80;
        case Token::Type::MN_ADM: return 0xEB;
        case Token::Type::MN_BBL: return 0xC0;
        case Token::Type::MN_CLB: return 0xF0;
        case Token::Type::MN_CLC: return 0xF1;
        case Token::Type::MN_CMA: return 0xF4;
        case Token::Type::MN_CMC: return 0xF3;
        case Token::Type::MN_DAA: return 0xFB;
        case Token::Type::MN_DAC: return 0xF8;
        case Token::Type::MN_DCL: return 0xFD;
        case Token::Type::MN_FIM: return 0x20;
        case Token::Type::MN_FIN: return 0x30;
        case Token::Type::MN_IAC: return 0xF2;
        case Token::Type::MN_INC: return 0x60;
        case Token::Type::MN_ISZ: return 0x70;
        case Token::Type::MN_JCN: return 0x10;
        case Token::Type::MN_JIN: return 0x31;
        case Token::Type::MN_JMS: return 0x50;
        case Token::Type::MN_JUN: return 0x40;
        case Token::Type::MN_KBP: return 0xFC;
        case Token::Type::MN_LD:  return 0xA0;
        case Token::Type::MN_LDM: return 0xD0;
        case Token::Type::MN_RAL: return 0xF5;
        case Token::Type::MN_RAR: return 0xF6;
        }

        return 0;
    }

    void Parser::line()
    {
        if (m_current.type < Token::Type::MNEMONIC_COUNT)
            instruction();

        if (match(Token::Type::Label))
        {
            advance();
            if (match(Token::Type::Comma))
            {
                advance();
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

        if (match(Token::Type::Equal))
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

        u16 value = expression();
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
        case Token::Type::MN_FIM: break;
        case Token::Type::MN_FIN: break;
        case Token::Type::MN_IAC: break;
        case Token::Type::MN_INC: break;
        case Token::Type::MN_ISZ: break;
        case Token::Type::MN_JCN: break;
        case Token::Type::MN_JIN: break;
        case Token::Type::MN_JMS: break;
        case Token::Type::MN_JUN:
            if (value > 0xFFF)
                errorAt(m_current, "Expression overflowed.");

            m_output.push_back(0x40 | (value >> 8));
            m_output.push_back(value & 0xFF);
            break;
        case Token::Type::MN_KBP: break;
        case Token::Type::MN_LD:  break;
        case Token::Type::MN_LDM: break;
        case Token::Type::MN_RAL: break;
        case Token::Type::MN_RAR: break;
        case Token::Type::MN_RDM: break;
        case Token::Type::MN_RDR: break;
        case Token::Type::MN_RDX: break;
        case Token::Type::MN_SBM: break;
        case Token::Type::MN_SRC: break;
        case Token::Type::MN_STC: break;
        case Token::Type::MN_SUB: break;
        case Token::Type::MN_TCC: break;
        case Token::Type::MN_TCS: break;
        case Token::Type::MN_WMP: break;
        case Token::Type::MN_WRM: break;
        case Token::Type::MN_WRR: break;
        case Token::Type::MN_WRX: break;
        case Token::Type::MN_XCH: break;
        }
    }

    u16 Parser::expression()
    {
        if (match(Token::Type::Label))
        {
            std::string_view str{ m_previous}
            m_symbolTable.contains()
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
