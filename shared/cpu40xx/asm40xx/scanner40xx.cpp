#include "scanner40xx.hpp"
#include "asm_common.hpp"

namespace ASM40xx
{

    Token Scanner::getToken() {
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

            return makeToken(Token::Type::Number);
        }

        switch (*current++)
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
            switch (*current)
            {
            case ' ':
            case '\r':
            case '\t':
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

    Token::Type Scanner::identifierType()
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

    Token::Type Scanner::checkMnemonic(int start, int length, const char* rest, Token::Type type)
    {
        if (current - this->start == start + length &&
            memcmp(this->start + start, rest, length) == 0) return type;
        return Token::Type::Label;
    }

    Token Scanner::makeToken(Token::Type type)
    {
        Token token;
        token.type = type;
        token.start = start;
        token.length = current - start;
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

}
