#include "cpu40xx/assembler40xx.hpp"

#include <utils/file_io.hpp>

#include <gtest/gtest.h>
#include <array>
#include <optional>

using namespace std::literals;

namespace {

struct AssemblerTestParam {
    std::string_view name;
    std::string_view source;
    std::string_view refHex;
    std::string_view regLog;
};

struct CPU40xxAssemblerParamTests :
    ::testing::TestWithParam<AssemblerTestParam> {};

TEST_P(CPU40xxAssemblerParamTests, GivenValidProgramSourceMWhenAssemblingStatusIsSuccessAndOutputAndLogIsCorrect) {
    Assembler40xx assembler{ GetParam().source };

    EXPECT_EQ(assembler.assemble(), Assembler40xx::Status::Success);
    std::string_view hex = assembler.getHex();
    std::string_view log = assembler.getLog();

    EXPECT_EQ(hex, GetParam().refHex);
    EXPECT_EQ(log, GetParam().regLog);
}

INSTANTIATE_TEST_SUITE_P(, CPU40xxAssemblerParamTests, ::testing::Values(
    AssemblerTestParam{ "Expressions", R"(
A =   0xB
  LDM 10
  LDM A
  LDM 0x5+02
  LDM 0b110-B
  LDM -010+10
B =   3
)",
    R"(:05000000DADBD7D3D2CA
:00000001FF)", "" },

    AssemblerTestParam{ "NOP", "NOP",
    R"(:0100000000FF
:00000001FF)", "" },

    AssemblerTestParam{ "LDM", "LDM 10",
    R"(:01000000DA25
:00000001FF)", "" },

    AssemblerTestParam{ "LD", R"(
LD 10
LD R8
)",
    R"(:02000000AAA8AC
:00000001FF)", "" },

    AssemblerTestParam{ "XCH", R"(
XCH 10
XCH R8
)",
    R"(:02000000BAB88C
:00000001FF)", "" },

    AssemblerTestParam{ "ADD", R"(
ADD 10
ADD R8
)",
    R"(:020000008A88EC
:00000001FF)", "" },

    AssemblerTestParam{ "SUB", R"(
SUB 10
SUB R8
)",
    R"(:020000009A98CC
:00000001FF)", "" },

    AssemblerTestParam{ "INC", R"(
INC 10
INC R8
)",
    R"(:020000006A682C
:00000001FF)", "" },

    AssemblerTestParam{ "BBL", "BBL 10",
    R"(:01000000CA35
:00000001FF)", "" },

    AssemblerTestParam{ "JIN", R"(
JIN 10
JIN P1
)",
    R"(:020000003B3390
:00000001FF)", "" },

    AssemblerTestParam{ "JUN", "JUN 0x123",
R"(:0200000041239A
:00000001FF)", "" },

    AssemblerTestParam{ "JMS", "JMS 0x123",
R"(:0200000051238A
:00000001FF)", "" },

    AssemblerTestParam{ "JCN", "JCN 0b1010 0x12",
R"(:020000001A12D2
:00000001FF)", "" },

    AssemblerTestParam{ "ISZ", R"(
ISZ 10 0x12
ISZ R8 0x34
)",
    R"(:040000007A127834C4
:00000001FF)", "" },

    AssemblerTestParam{ "FIM", R"(
FIM 10 0x12
FIM P1 0x34
)",
    R"(:040000002A1222346A
:00000001FF)", "" },

    AssemblerTestParam{ "RDM", "RDM",
    R"(:01000000E916
:00000001FF)", "" },

    AssemblerTestParam{ "RDx", R"(
RD0
RD1
RD2
RD3
)",
    R"(:04000000ECEDEEEF46
:00000001FF)", "" },

    AssemblerTestParam{ "EmptySource", "", ":00000001FF", "Warning[0]: source is empty.\n" },
    AssemblerTestParam{ "4bitOverflow", "LDM 0x123",
                        ":01000000D32C\n:00000001FF",
                        "Warning[1]: expression truncated to fit argument.\n" }
    ),

    [](const testing::TestParamInfo<CPU40xxAssemblerParamTests::ParamType>& paramInfo) {
        return std::string(paramInfo.param.name);
    }
);

} // namespace
