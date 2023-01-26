#pragma once
#include "scanner40xx.hpp"

#include <string_view>
#include <unordered_map>
#include <vector>

namespace ASM40xx {

    class Parser
    {
    public:
        explicit Parser(const char* source);

        bool parse();
    private:
        void advance();
        void line();
        bool match(Token::Type type);
        void errorAt(const Token& token, const char* message);
        void consume(Token::Type type, const char* message);
        void instruction();
        u16 expression();

        Scanner m_scanner;

        Token m_current{};
        Token m_previous{};
        u16 m_address;
        bool m_hadError;
        bool m_panicMode;
        int m_line;
        std::vector<u8> m_output;
        std::unordered_map<std::string_view, u16> m_symbolTable;
    };

}
