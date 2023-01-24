#include "scanner40xx.hpp"
#include "asm_common.hpp"

namespace ASM40xx {

    Token Scanner::getToken() {
        skipWhitespace();
        m_start = m_current;
        if (*m_current == '\0') return makeToken(Token::Type::EndOfSource);

        if (isAlpha(*m_current))
        {
            while (isAlpha(*m_current) || isDigit(*m_current)) m_current++;
            return makeToken(identifierType());
        }

        if (isDigit(*m_current))
        {
            while (isDigit(*m_current)) m_current++;
            return numberToken();
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

    Token::Type Scanner::identifierType()
    {
        switch (m_start[0]) {
        case 'A':
            if (m_current - m_start > 1 && m_start[1] == 'D' && m_current - m_start > 2)
                switch (m_start[2])
                {
                case 'D': return checkMnemonic(3, 0, "", Token::Type::MN_ADD);
                case 'M': return checkMnemonic(3, 0, "", Token::Type::MN_ADM);
                }
            break;
        case 'B': return checkMnemonic(1, 2, "BL", Token::Type::MN_BBL);
        case 'C':
            if (m_current - m_start > 1)
                switch (m_start[1])
                {
                case 'L':
                    if (m_current - m_start > 2)
                        switch (m_start[2])
                        {
                        case 'B': return checkMnemonic(3, 0, "", Token::Type::MN_CLB);
                        case 'C': return checkMnemonic(3, 0, "", Token::Type::MN_CLC);
                        }
                    break;
                case 'M':
                    if (m_current - m_start > 2)
                        switch (m_start[2])
                        {
                        case 'A': return checkMnemonic(3, 0, "", Token::Type::MN_CMA);
                        case 'C': return checkMnemonic(3, 0, "", Token::Type::MN_CMC);
                        }
                    break;
                }
            break;
        case 'D':
            if (m_current - m_start > 1)
                switch (m_start[1])
                {
                case 'A':
                    if (m_current - m_start > 2)
                        switch (m_start[2])
                        {
                        case 'A': return checkMnemonic(3, 0, "", Token::Type::MN_DAA);
                        case 'C': return checkMnemonic(3, 0, "", Token::Type::MN_DAC);
                        }
                    break;
                case 'C': return checkMnemonic(1, 2, "CL", Token::Type::MN_DCL);
                }
            break;
        case 'F':
            if (m_current - m_start > 1 && m_start[1] == 'I' && m_current - m_start > 2)
                switch (m_start[2])
                {
                case 'M': return checkMnemonic(3, 0, "", Token::Type::MN_FIM);
                case 'N': return checkMnemonic(3, 0, "", Token::Type::MN_FIN);
                }
            break;
        case 'I':
            if (m_current - m_start > 1)
                switch (m_start[1])
                {
                case 'A': return checkMnemonic(2, 1, "C", Token::Type::MN_IAC);
                case 'N': return checkMnemonic(2, 1, "C", Token::Type::MN_INC);
                case 'S': return checkMnemonic(2, 1, "Z", Token::Type::MN_ISZ);
                }
            break;
        case 'J':
            if (m_current - m_start > 1)
                switch (m_start[1])
                {
                case 'C': return checkMnemonic(2, 1, "N", Token::Type::MN_JCN);
                case 'I': return checkMnemonic(2, 1, "N", Token::Type::MN_JIN);
                case 'M': return checkMnemonic(2, 1, "S", Token::Type::MN_JMS);
                case 'U': return checkMnemonic(2, 1, "N", Token::Type::MN_JUN);
                }
            break;
        case 'K': return checkMnemonic(1, 2, "BP", Token::Type::MN_KBP);
        case 'L':
            if (m_current - m_start > 1 && m_start[1] == 'D')
            {
                if (m_current - m_start > 2 && m_start[2] == 'M')
                    return checkMnemonic(3, 0, "", Token::Type::MN_LDM);

                return checkMnemonic(2, 0, "", Token::Type::MN_LD);
            }
            break;
        case 'N': return checkMnemonic(1, 2, "OP", Token::Type::MN_NOP);
        case 'R':
            if (m_current - m_start > 1)
                switch (m_start[1])
                {
                case 'A':
                    if (m_current - m_start > 2)
                        switch (m_start[2])
                        {
                        case 'L': return checkMnemonic(3, 0, "", Token::Type::MN_RAL);
                        case 'R': return checkMnemonic(3, 0, "", Token::Type::MN_RAR);
                        }
                    break;
                case 'D':
                    if (m_current - m_start > 2)
                        switch (m_start[2])
                        {
                        case 'M': return checkMnemonic(3, 0, "", Token::Type::MN_RDM);
                        case 'R': return checkMnemonic(3, 0, "", Token::Type::MN_RDR);
                        case 'X': return checkMnemonic(3, 0, "", Token::Type::MN_RDX);
                        }
                    break;
                }
            break;
        case 'S':
            if (m_current - m_start > 1)
                switch (m_start[1])
                {
                case 'B': return checkMnemonic(2, 1, "M", Token::Type::MN_SBM);
                case 'R': return checkMnemonic(2, 1, "C", Token::Type::MN_SRC);
                case 'T': return checkMnemonic(2, 1, "C", Token::Type::MN_STC);
                case 'U': return checkMnemonic(2, 1, "B", Token::Type::MN_SUB);
                }
            break;
        case 'T':
            if (m_current - m_start > 1 && m_start[1] == 'C' && m_current - m_start > 2)
                switch (m_start[2])
                {
                case 'C': return checkMnemonic(3, 0, "", Token::Type::MN_TCC);
                case 'S': return checkMnemonic(3, 0, "", Token::Type::MN_TCS);
                }
            break;
        case 'W':
            if (m_current - m_start > 1)
                switch (m_start[1])
                {
                case 'M': return checkMnemonic(2, 1, "P", Token::Type::MN_WMP);
                case 'R':
                    switch (m_start[2])
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

    Token::Type Scanner::checkMnemonic(int m_start, int length, const char* rest, Token::Type type)
    {
        if (m_current - this->m_start == m_start + length &&
            memcmp(this->m_start + m_start, rest, length) == 0) return type;
        return Token::Type::Label;
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
        token.type = Token::Type::Number;
        token.start = m_start;
        token.length = m_current - m_start;

        u16 value = 0;
        if (m_current[0] == 'H')
        {
            m_current++;

            u16 pos = 1;
            for (int i = token.length - 1; i >= 0; i--)
            {
                value += digitCharToValue(token.start[i]) * pos;
                pos *= 16;
            }
        }
        else
        {
            u16 pos = 1;
            for (int i = token.length - 1; i >= 0; i--)
            {
                value += digitCharToValue(token.start[i]) * pos;
                pos *= 10;
            }
        }
        token.value = value;

        return token;
    }

    bool Scanner::isDigit(char c)
    {
        return (c >= '0' && c <= '9') ||
               (c >= 'A' && c <= 'F');
    }

    bool Scanner::isAlpha(char c)
    {
        return (c >= 'A' && c <= 'Z') ||
                c == '_';
    }

}
