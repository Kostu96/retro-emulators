#pragma once
#include "asm_common.hpp"
#include "trie.hpp"

#include <vector>

namespace ASM8008 {

    struct Token
    {
        enum class Type {
            MN_ACI, MN_ADC, MN_ADD, MN_ADI, MN_ANA, MN_ANI,
            MN_CALL, MN_CC, MN_CM, MN_CMP, MN_CNC, MN_CNZ, MN_CP, MN_CPE, MN_CPI, MN_CPO, MN_CZ,
            MN_DB, MN_DCR, MN_DS, MN_DW,
            MN_END, MN_ENDIF, MN_ENDM, MN_EQU,
            MN_HLT,
            MN_IF, MN_IN, MN_INR,
            MN_JC, MN_JM, MN_JMP, MN_JNC, MN_JNZ, MN_JP, MN_JPE, MN_JPO, MN_JZ,
            MN_MACRO, MN_MOV, MN_MVI,
            MN_NOP,
            MN_ORA, MN_ORG, MN_ORI, MN_OUT,
            MN_RAL, MN_RAR, MN_RC, MN_RET, MN_RFC, MN_RLC, MN_RM, MN_RNC, MN_RNZ, MN_RP, MN_RPE, MN_RPO, MN_RRC, MN_RST, MN_RZ,
            MN_SBB, MN_SBI, MN_SET, MN_SUB, MN_SUI,
            MN_XRA, MN_XRI,
            MN_Count,

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
        Scanner(const char* source, std::vector<ErrorMessage>& errors);

        Token getToken();
    private:
        Token makeToken(Token::Type type);
        Token numberToken();
        void skipWhitespacesAndComments();

        const char* m_start;
        const char* m_current;
        std::vector<ErrorMessage>& m_errors;
        Trie<Token::Type> m_mnemonics;
    };

}
