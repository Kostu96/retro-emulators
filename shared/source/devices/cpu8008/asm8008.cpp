#include "shared/source/devices/cpu8008/asm8008.hpp"
#include "shared/source/devices/cpu8008/scanner8008.hpp"
#include "shared/source/disassembly_line.hpp"

#include <cassert>
#include <iomanip>
#include <unordered_map>

#define PRINT1 printBytes(ss, code, addr, 1, &byte1)
#define PRINT2 printBytes(ss, code, addr, 2, &byte1, &byte2)
#define INST1(str) ss << "       " str; break
#define INST2(str) PRINT1; ss << "    " str << std::setw(2) << (u16)byte1; break
#define INST3(str) PRINT2; ss << " " str << std::setw(2) << (u16)byte2 << std::setw(2) << (u16)byte1; break

namespace ASM8008
{

    void assemble(const char* source, std::vector<u8>& /*output*/, std::vector<ErrorMessage>& errors)
    {
        Scanner scanner{ source, errors };
        std::unordered_map<std::string_view, u16> labels;

        u32 line = 1;
        u16 address = 0;
        Token token = scanner.getToken();
        while (token.type != Token::Type::MN_END)
        {
            switch (token.type)
            {
            case Token::Type::Unknown:
                errors.push_back({ line, "Unexpected character." });
                break;
            case Token::Type::EndOfSource:
                errors.push_back({ line, "Expected 'END' as the last mnemonic of the source." });
                break;
            case Token::Type::EndOfLine:
                line++;
                break;
            case Token::Type::Label: {
                std::string_view str{ token.start, token.length };

                token = scanner.getToken();
                if (token.type != Token::Type::Colon)
                    errors.push_back({ line, "Unterminated label." });

                labels.emplace(str, address);
            } break;
            default:
                if (token.type < Token::Type::MN_Count)
                {

                }
                else
                {
                    errors.push_back({ line, "Unexpected token." });
                }
            }

            token = scanner.getToken();
        }
    }

    void disassemble(const u8* code, size_t code_size, std::vector<DisassemblyLine>& output)
    {
        for (size_t addr = 0; addr < code_size; )
        {
            DisassemblyLine line;
            line.address = (u16)addr;
            std::stringstream ss;
            ss << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << addr << ":  ";
            u8 byte = code[addr++];
            ss << std::hex << std::setw(2) << (u16)byte << ' ';

            /*u8 byte1, byte2;
            switch (byte)
            {

            default: INST1("???");
            }*/

            assert(ss.str().size() <= sizeof(line.buffer));
            strcpy_s(line.buffer, ss.str().c_str());
            output.push_back(line);
        }
    }

}
