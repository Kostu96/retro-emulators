#include "cpu40xx/assembler40xx.hpp"

#include <utils/file_io.hpp>

#include <gtest/gtest.h>
#include <array>
#include <optional>

using namespace std::literals;

namespace {

TEST(CPU40xxAssemblerPositiveTests, GivenValidProgramSourceWithNOPWhenAssemblingStatusIsSuccessAndOutputIsCorrect) {
    const char* source = "NOP";

    Assembler40xx assembler{ source };

    EXPECT_EQ(assembler.assemble(), Assembler40xx::Status::Success);
    std::string_view hex = assembler.getHex();
    std::string_view log = assembler.getLog();

    std::string_view refHex = R"(:0100000000FF
:00000001FF)";

    EXPECT_EQ(hex, refHex);
    EXPECT_EQ(log, ""sv);
}

TEST(CPU40xxAssemblerPositiveTests, GivenValidProgramSourceWithLDMWhenAssemblingStatusIsSuccessAndOutputIsCorrect) {
    const char* source = R"(
A =   0xB
  LDM 10
  LDM A
  LDM 0x5+02
  LDM 0b110-B
  LDM -010+10
B =   3
)";

    Assembler40xx assembler{ source };

    EXPECT_EQ(assembler.assemble(), Assembler40xx::Status::Success);
    std::string_view hex = assembler.getHex();
    std::string_view log = assembler.getLog();

    std::string_view refHex = R"(:05000000DADBD7D3D2CA
:00000001FF)";

    EXPECT_EQ(hex, refHex);
    EXPECT_EQ(log, ""sv);
}

TEST(CPU40xxAssemblerWarningTests, GivenEmptyProgramSourceWhenAssemblingStatusIsSuccessOutputIsCorrectAndLogHasWarning) {
    const char* source = R"()";

    Assembler40xx assembler{ source };

    EXPECT_EQ(assembler.assemble(), Assembler40xx::Status::Success);
    std::string_view hex = assembler.getHex();
    std::string_view log = assembler.getLog();

    std::string_view refHex = R"(:00000001FF)";

    EXPECT_EQ(hex, refHex);
    EXPECT_EQ(log, "Warning[0]: source is empty."sv);
}

//std::string readSourceForTest(const char* testName) {
//    std::string path = "test_files/cpu40xx/"s + testName + ".asm";
//    return readFile(path.c_str());
//}
//
//std::string readHexForTest(const char* testName) {
//    std::string path = "test_files/cpu40xx/"s + testName + "_ref.hex";
//    return readFile(path.c_str());
//}
//
//TEST(CPU40xxAssemblerPositiveTests, GivenValidProgramSourceWhenAssemblingStatusIsSuccessAndOutputIsCorrect) {
//    const char* testName = "simple_addition";
//    
//    std::string source = readSourceForTest(testName);
//    Assembler40xx assembler{ source.c_str() };
//    
//    EXPECT_EQ(assembler.assemble(), Assembler40xx::Status::Success);
//    std::string_view hex = assembler.getHex();
//
//    std::string refHex = readHexForTest(testName);
//    EXPECT_EQ(hex, refHex);
//}
//
//TEST(CPU40xxAssemblerNegativeTests, GivenIvalidProgramSourceWhenAssemblingStatusIsErrorAndCorrectLogIsProduced) {
//    //ASSERT_TRUE(assembler.has_value());
//    //EXPECT_EQ(assembler->assemble(), Assembler40xx::Status::Error);
//}

} // namespace
