#pragma once
#include "scanner40xx.hpp"

namespace ASM40xx {

    class Parser
    {
    public:
        explicit Parser(const char* source);

        void advance();
    private:
        void errorAt(const Token& token, const char* message, int line);

        Scanner m_scanner;

        Token m_current{};
        Token m_previous{};
        bool m_hadError;
        int m_line;
    };

}
