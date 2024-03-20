#pragma once
#include "shared/source/asm/trie.hpp"
#include "shared/source/types.hpp"

namespace ASM40xx {

    struct Token
    {
        enum class Type
        {
            MN_ADD, MN_ADM, MN_AN6, MN_AN7,
            MN_BBL, MN_BBS,
            MN_CLB, MN_CLC, MN_CMA, MN_CMC,
            MN_DAA, MN_DAC, MN_DB0, MN_DB1, MN_DCL, MN_DIN,
            MN_EIN,
            MN_FIM, MN_FIN,
            MN_HLT,
            MN_IAC, MN_INC, MN_ISZ,
            MN_JCN, MN_JIN, MN_JMS, MN_JUN,
            MN_KBP,
            MN_LCR, MN_LD, MN_LDM,
            MN_NOP,
            MN_OR4, MN_OR5,
            MN_RAL, MN_RAR, MN_RDM, MN_RDR, MN_RD0, MN_RD1, MN_RD2, MN_RD3, MN_RPM,
            MN_SBM, MN_SB0, MN_SB1, MN_SRC, MN_STC, MN_SUB,
            MN_TCC, MN_TCS,
            MN_WMP, MN_WPM, MN_WRM, MN_WRR, MN_WR0, MN_WR1, MN_WR2, MN_WR3,
            MN_XCH,
            MNEMONIC_COUNT,

            Label,
            Number,
            Plus,
            Minus,
            Star,
            Comma,
            Equal,
            Error,
            EndOfLine,
            EndOfSource,
        };

        Type type = Type::Error;
        const char* start = nullptr;
        size_t length = 0;
        u16 value = 0;
    };

    class Scanner
    {
    public:
        explicit Scanner(const char* source);

        Token getToken();
    private:
        void skipWhitespace();
        Token makeToken(Token::Type type);
        Token errorToken(const char* message);
        Token numberToken();

        const char* m_start;
        const char* m_current;
        Trie<Token::Type> m_mnemonics;
    };

}
