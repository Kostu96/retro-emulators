#pragma once
#include "asm_common.hpp"

#include <vector>

namespace ASM8008 {

    struct Token
    {
        enum class Type {
            MN_HLT,
            MN_COUNT,

            Colon,
            Comma,
            Dollar,
            LeftParen, RightParen,
            Plus, Minus, Star, Slash, Mod,
            Not, And, Or, Xor, Shr, Shl,

            Number,
            Label,

            EndOfLine,
            EndOfSource,
            Unknown
        };
        
        Type type;
        const char* start;
        size_t length;
        u16 value;
    };

    class Scanner
    {
    public:
        Scanner(const char* source, std::vector<ErrorMessage>& errors) :
            m_start{ source },
            m_current{ source },
            m_errors{ errors } {}

        Token getToken();
    private:
        Token makeToken(Token::Type type);
        Token numberToken();
        Token::Type identifierType();
        Token::Type checkMnemonic(int m_start, int length, const char* rest, Token::Type type);
        void skipWhitespacesAndComments();
        bool isDigit(char c);
        bool isAlpha(char c);

        const char* m_start;
        const char* m_current;
        std::vector<ErrorMessage>& m_errors;
    };

}
