#include "parser40xx.hpp"

#include <cstdio>

namespace ASM40xx {

    Parser::Parser(const char* source) :
        m_scanner{ source },
        m_line{ 1 } {}

    void Parser::advance()
    {
        m_previous = m_current;

        while (true)
        {
            m_current = m_scanner.getToken();
            if (m_current.type != Token::Type::Error) break;

            errorAt(m_current, m_current.start, m_line);
        }
    }

    void Parser::errorAt(const Token& token, const char* message, int line)
    {
        fprintf(stderr, "[line %d] Error", line);

        if (token.type == Token::Type::EndOfSource) {
            fprintf(stderr, " at end");
        }
        else if (token.type == Token::Type::Error) {
            // Nothing.
        }
        else {
            fprintf(stderr, " at '%.*s'", token.length, token.start);
        }

        fprintf(stderr, ": %s\n", message);
        m_hadError = true;
    }

}
