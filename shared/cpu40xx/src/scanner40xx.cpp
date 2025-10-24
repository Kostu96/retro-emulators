#include "scanner40xx.hpp"
#include "utils/asm_common.hpp"

namespace ASM40xx {

    Scanner::Scanner(const char* source) :
        m_start{ source },
        m_current{ source }
    {
        m_mnemonics.emplace("ADD", Token::Type::MN_ADD);
        m_mnemonics.emplace("ADM", Token::Type::MN_ADM);
        m_mnemonics.emplace("AN6", Token::Type::MN_AN6);
        m_mnemonics.emplace("AN7", Token::Type::MN_AN7);
        m_mnemonics.emplace("BBL", Token::Type::MN_BBL);
        m_mnemonics.emplace("BBS", Token::Type::MN_BBS);
        m_mnemonics.emplace("CLB", Token::Type::MN_CLB);
        m_mnemonics.emplace("CLC", Token::Type::MN_CLC);
        m_mnemonics.emplace("CMA", Token::Type::MN_CMA);
        m_mnemonics.emplace("CMC", Token::Type::MN_CMC);
        m_mnemonics.emplace("DAA", Token::Type::MN_DAA);
        m_mnemonics.emplace("DAC", Token::Type::MN_DAC);
        m_mnemonics.emplace("DB0", Token::Type::MN_DB0);
        m_mnemonics.emplace("DB1", Token::Type::MN_DB1);
        m_mnemonics.emplace("DCL", Token::Type::MN_DCL);
        m_mnemonics.emplace("DIN", Token::Type::MN_DIN);
        m_mnemonics.emplace("EIN", Token::Type::MN_EIN);
        m_mnemonics.emplace("FIM", Token::Type::MN_FIM);
        m_mnemonics.emplace("FIN", Token::Type::MN_FIN);
        m_mnemonics.emplace("HLT", Token::Type::MN_HLT);
        m_mnemonics.emplace("IAC", Token::Type::MN_IAC);
        m_mnemonics.emplace("INC", Token::Type::MN_INC);
        m_mnemonics.emplace("ISZ", Token::Type::MN_ISZ);
        m_mnemonics.emplace("JCN", Token::Type::MN_JCN);
        m_mnemonics.emplace("JIN", Token::Type::MN_JIN);
        m_mnemonics.emplace("JMS", Token::Type::MN_JMS);
        m_mnemonics.emplace("JUN", Token::Type::MN_JUN);
        m_mnemonics.emplace("KBP", Token::Type::MN_KBP);
        m_mnemonics.emplace("LCR", Token::Type::MN_LCR);
        m_mnemonics.emplace("LD",  Token::Type::MN_LD);
        m_mnemonics.emplace("LDM", Token::Type::MN_LDM);
        m_mnemonics.emplace("NOP", Token::Type::MN_NOP);
        m_mnemonics.emplace("OR4", Token::Type::MN_OR4);
        m_mnemonics.emplace("OR5", Token::Type::MN_OR5);
        m_mnemonics.emplace("RAL", Token::Type::MN_RAL);
        m_mnemonics.emplace("RAR", Token::Type::MN_RAR);
        m_mnemonics.emplace("RDM", Token::Type::MN_RDM);
        m_mnemonics.emplace("RDR", Token::Type::MN_RDR);
        m_mnemonics.emplace("RD0", Token::Type::MN_RD0);
        m_mnemonics.emplace("RD1", Token::Type::MN_RD1);
        m_mnemonics.emplace("RD2", Token::Type::MN_RD2);
        m_mnemonics.emplace("RD3", Token::Type::MN_RD3);
        m_mnemonics.emplace("RPM", Token::Type::MN_RPM);
        m_mnemonics.emplace("SBM", Token::Type::MN_SBM);
        m_mnemonics.emplace("SB0", Token::Type::MN_SB0);
        m_mnemonics.emplace("SB1", Token::Type::MN_SB1);
        m_mnemonics.emplace("SRC", Token::Type::MN_SRC);
        m_mnemonics.emplace("STC", Token::Type::MN_STC);
        m_mnemonics.emplace("SUB", Token::Type::MN_SUB);
        m_mnemonics.emplace("TCC", Token::Type::MN_TCC);
        m_mnemonics.emplace("TCS", Token::Type::MN_TCS);
        m_mnemonics.emplace("WMP", Token::Type::MN_WMP);
        m_mnemonics.emplace("WPM", Token::Type::MN_WPM);
        m_mnemonics.emplace("WRM", Token::Type::MN_WRM);
        m_mnemonics.emplace("WRR", Token::Type::MN_WRR);
        m_mnemonics.emplace("WR0", Token::Type::MN_WR0);
        m_mnemonics.emplace("WR1", Token::Type::MN_WR1);
        m_mnemonics.emplace("WR2", Token::Type::MN_WR2);
        m_mnemonics.emplace("WR3", Token::Type::MN_WR3);
        m_mnemonics.emplace("XCH", Token::Type::MN_XCH);
    }

    Token Scanner::getToken() {
        skipWhitespace();
        m_start = m_current;
        if (*m_current == '\0') return makeToken(Token::Type::EndOfSource);

        if (isDigit(*m_current))
        {
            while (isDigit(*m_current)) m_current++;
            return numberToken();
        }

        if (isAlpha(*m_current))
        {
            while (isAlpha(*m_current) || isDigit(*m_current)) m_current++;

            std::string_view str{ m_start, (size_t)(m_current - m_start) };
            Token::Type type;
            if (m_mnemonics.find(str, type))
                return makeToken(type);

            return makeToken(Token::Type::Label);
        }

        switch (*m_current++)
        {
        case '\n': return makeToken(Token::Type::EndOfLine);
        case '+': return makeToken(Token::Type::Plus);
        case '-': return makeToken(Token::Type::Minus);
        case '*': return makeToken(Token::Type::Star);
        case ',': return makeToken(Token::Type::Comma);
        case '=': return makeToken(Token::Type::Equal);
        }

        return errorToken("Unexpected character.");
    }

    void Scanner::skipWhitespace() {
        for (;;)
            switch (*m_current)
            {
            case ' ':
            case '\r':
            case '\t':
                m_current++;
                break;
            case '/':
                while (*m_current != '\n' && *m_current != '\0')
                    m_current++;
                break;
            default:
                return;
            }
    }

    Token Scanner::makeToken(Token::Type type)
    {
        Token token;
        token.type = type;
        token.start = m_start;
        token.length = m_current - m_start;
        return token;
    }

    Token Scanner::errorToken(const char* message)
    {
        Token token;
        token.type = Token::Type::Error;
        token.start = message;
        token.length = std::strlen(message);
        return token;
    }

    Token Scanner::numberToken()
    {
        Token token;
        token.start = m_start;
        token.length = m_current - m_start;
        
        bool error = false;
        if (m_current[0] == 'H')
        {
            m_current++;
            error = !stringToValue(token.start, token.length, NumberBase::Hex, token.value);
        }
        else if (m_current[0] == 'Q' || m_current[0] == 'O')
        {
            m_current++;
            error = !stringToValue(token.start, token.length, NumberBase::Oct, token.value);
        }
        else if (m_current[0] == 'B')
        {
            m_current++;
            error = !stringToValue(token.start, token.length, NumberBase::Bin, token.value);
        }
        else
            error = !stringToValue(token.start, token.length, NumberBase::Dec, token.value);

        token.type = error ? Token::Type::Error : Token::Type::Number;

        return token;
    }

}
