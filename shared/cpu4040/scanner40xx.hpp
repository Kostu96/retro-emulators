#pragma once
#include "type_aliases.hpp"

namespace ASM40xx
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
            MN_LD, MN_LDM,
            MN_RAL, MN_RAR, MN_RDM, MN_RDR, MN_RDX,
            MN_SBM, MN_SRC, MN_STC, MN_SUB,
            MN_TCC, MN_TCS,
            MN_WMP, MN_WRM, MN_WRR, MN_WRX,
            MN_XCH,
            MNEMONIC_COUNT,

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

        Token getToken();
    private:
        void skipWhitespace();
        Token::Type identifierType();
        Token::Type checkMnemonic(int start, int length, const char* rest, Token::Type type);

        Token makeToken(Token::Type type)
        {
            Token token;
            token.type = type;
            token.start = start;
            token.length = current - start;
            token.line = line;
            return token;
        }

        const char* start;
        const char* current;
        u32 line;
    };

}
