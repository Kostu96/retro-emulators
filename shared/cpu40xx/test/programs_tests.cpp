#include "cpu4004_test_fixture.hpp"

namespace {

using CPU4004ProgramsTests = CPU4004Tests;

// TODO(Kostu): assembler test programs? separate cpp file?
TEST_F(CPU4004ProgramsTests, SubroutineTest) {
    // TODO(Kostu):
    GTEST_SKIP();
}

TEST_F(CPU4004ProgramsTests, CounterLoopTest) {
    // TODO(Kostu): 8 bit ISZ loop
    GTEST_SKIP();
}

TEST_F(CPU4004ProgramsTests, IntegerBCDMathTest) {
    // TODO(Kostu):
    GTEST_SKIP();
}

TEST_F(CPU4004ProgramsTests, FloatingPointBCDMathTest) {
    // TODO(Kostu):
    GTEST_SKIP();
}

TEST_F(CPU4004ProgramsTests, RAMAccessTest) {
    // TODO(Kostu): DCL, SRC, and instructions
    GTEST_SKIP();
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
