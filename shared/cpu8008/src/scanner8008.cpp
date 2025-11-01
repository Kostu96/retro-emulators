#include "scanner8008.hpp"

namespace ASM8008 {

    Scanner::Scanner(const char* source, std::vector<ErrorMessage>& errors) :
        m_start{ source },
        m_current{ source },
        m_errors{ errors }
    {
        m_mnemonics.emplace("ACI",   Token::Type::MN_ACI);
        m_mnemonics.emplace("ADC",   Token::Type::MN_ADC);
        m_mnemonics.emplace("ADD",   Token::Type::MN_ADD);
        m_mnemonics.emplace("ADI",   Token::Type::MN_ADI);
        m_mnemonics.emplace("ANA",   Token::Type::MN_ANA);
        m_mnemonics.emplace("ANI",   Token::Type::MN_ANI);
        m_mnemonics.emplace("CALL",  Token::Type::MN_CALL);
        m_mnemonics.emplace("CC",    Token::Type::MN_CC);
        m_mnemonics.emplace("CM",    Token::Type::MN_CM);
        m_mnemonics.emplace("CMP",   Token::Type::MN_CMP);
        m_mnemonics.emplace("CNC",   Token::Type::MN_CNC);
        m_mnemonics.emplace("CNZ",   Token::Type::MN_CNZ);
        m_mnemonics.emplace("CP",    Token::Type::MN_CP);
        m_mnemonics.emplace("CPE",   Token::Type::MN_CPE);
        m_mnemonics.emplace("CPI",   Token::Type::MN_CPI);
        m_mnemonics.emplace("CPO",   Token::Type::MN_CPO);
        m_mnemonics.emplace("CZ",    Token::Type::MN_CZ);
        m_mnemonics.emplace("DB",    Token::Type::MN_DB);
        m_mnemonics.emplace("DCR",   Token::Type::MN_DCR);
        m_mnemonics.emplace("DS",    Token::Type::MN_DS);
        m_mnemonics.emplace("DW",    Token::Type::MN_DW);
        m_mnemonics.emplace("END",   Token::Type::MN_END);
        m_mnemonics.emplace("ENDIF", Token::Type::MN_ENDIF);
        m_mnemonics.emplace("ENDM",  Token::Type::MN_ENDM);
        m_mnemonics.emplace("EQU",   Token::Type::MN_EQU);
        m_mnemonics.emplace("HLT",   Token::Type::MN_HLT);
        m_mnemonics.emplace("IF",    Token::Type::MN_IF);
        m_mnemonics.emplace("IN",    Token::Type::MN_IN);
        m_mnemonics.emplace("INR",   Token::Type::MN_INR);
        m_mnemonics.emplace("JC",    Token::Type::MN_JC);
        m_mnemonics.emplace("JM",    Token::Type::MN_JM);
        m_mnemonics.emplace("JMP",   Token::Type::MN_JMP);
        m_mnemonics.emplace("JNC",   Token::Type::MN_JNC);
        m_mnemonics.emplace("JNZ",   Token::Type::MN_JNZ);
        m_mnemonics.emplace("JP",    Token::Type::MN_JP);
        m_mnemonics.emplace("JPE",   Token::Type::MN_JPE);
        m_mnemonics.emplace("JPO",   Token::Type::MN_JPO);
        m_mnemonics.emplace("JZ",    Token::Type::MN_JZ);
        m_mnemonics.emplace("MACRO", Token::Type::MN_MACRO);
        m_mnemonics.emplace("MOV",   Token::Type::MN_MOV);
        m_mnemonics.emplace("MVI",   Token::Type::MN_MVI);
        m_mnemonics.emplace("NOP",   Token::Type::MN_NOP);
        m_mnemonics.emplace("ORA",   Token::Type::MN_ORA);
        m_mnemonics.emplace("ORG",   Token::Type::MN_ORG);
        m_mnemonics.emplace("ORI",   Token::Type::MN_ORI);
        m_mnemonics.emplace("OUT",   Token::Type::MN_OUT);
        m_mnemonics.emplace("RAL",   Token::Type::MN_RAL);
        m_mnemonics.emplace("RAR",   Token::Type::MN_RAR);
        m_mnemonics.emplace("RC",    Token::Type::MN_RC);
        m_mnemonics.emplace("RET",   Token::Type::MN_RET);
        m_mnemonics.emplace("RFC",   Token::Type::MN_RFC);
        m_mnemonics.emplace("RLC",   Token::Type::MN_RLC);
        m_mnemonics.emplace("RM",    Token::Type::MN_RM);
        m_mnemonics.emplace("RNC",   Token::Type::MN_RNC);
        m_mnemonics.emplace("RNZ",   Token::Type::MN_RNZ);
        m_mnemonics.emplace("RP",    Token::Type::MN_RP);
        m_mnemonics.emplace("RPE",   Token::Type::MN_RPE);
        m_mnemonics.emplace("RPO",   Token::Type::MN_RPO);
        m_mnemonics.emplace("RRC",   Token::Type::MN_RRC);
        m_mnemonics.emplace("RST",   Token::Type::MN_RST);
        m_mnemonics.emplace("RZ",    Token::Type::MN_RZ);
        m_mnemonics.emplace("SBB",   Token::Type::MN_SBB);
        m_mnemonics.emplace("SBI",   Token::Type::MN_SBI);
        m_mnemonics.emplace("SET",   Token::Type::MN_SET);
        m_mnemonics.emplace("SUB",   Token::Type::MN_SUB);
        m_mnemonics.emplace("SUI",   Token::Type::MN_SUI);
        m_mnemonics.emplace("XRA",   Token::Type::MN_XRA);
        m_mnemonics.emplace("XRI",   Token::Type::MN_XRI);
    }

    Token Scanner::getToken()
    {
        skipWhitespacesAndComments();
        m_start = m_current;
        if (*m_current == '\0') return makeToken(Token::Type::EndOfSource);

        if (isDigit(*m_current))
        {
            while (isDigit(*m_current) || isAlpha(*m_current)) m_current++;
            return numberToken();
        }

        if (isAlpha(*m_current) || *m_current == '@' || *m_current == '?')
        {
            while (isAlpha(*m_current) || isDigit(*m_current)) m_current++;

            std::string_view str{ m_start, static_cast<size_t>(m_current - m_start) };
            Token::Type type;
            if (m_mnemonics.find(str, type))
                return makeToken(type);

            return makeToken(Token::Type::Label);
        }

        switch (*m_current++)
        {
        case ':':  return makeToken(Token::Type::Colon);
        case ',':  return makeToken(Token::Type::Comma);
        case '$':  return makeToken(Token::Type::Dollar);
        case '(':  return makeToken(Token::Type::LeftParen);
        case ')':  return makeToken(Token::Type::RightParen);
        case '+':  return makeToken(Token::Type::Plus);
        case '-':  return makeToken(Token::Type::Minus);
        case '*':  return makeToken(Token::Type::Star);
        case '/':  return makeToken(Token::Type::Slash);
        case '\n': return makeToken(Token::Type::EndOfLine);
        }

        return makeToken(Token::Type::Unknown);
    }

    Token Scanner::makeToken(Token::Type type)
    {
        Token token;
        token.type = type;
        token.start = m_start;
        token.length = m_current - m_start;
        return token;
    }

    Token Scanner::numberToken()
    {
        Token token;
        token.type = Token::Type::Number;
        token.start = m_start;
        token.length = m_current - m_start;
        token.value = 0;
        return token;
    }

    void Scanner::skipWhitespacesAndComments()
    {
        while (true)
            switch (*m_current)
            {
            case ' ':
            case '\r':
            case '\t':
                m_current++;
                break;
            case ';':
                while (*m_current != '\n' && *m_current != '\0')
                    m_current++;
                break;
            default:
                return;
            }
    }

}
