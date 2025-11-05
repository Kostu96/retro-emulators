set(CPU40XX_LIB_TARGET_NAME cpu40xx)

add_device_library(TARGET_NAME ${CPU40XX_LIB_TARGET_NAME}
    SOURCES
        CMakeLists.txt
        src/assembler40xx.cpp
        src/cpu40xx.cpp
        src/include/cpu40xx/assembler40xx.hpp
        src/include/cpu40xx/cpu40xx.hpp
    INCLUDE_DIRS
        src/include
    LIBRARIES
        utils
)

add_device_test(TARGET_NAME ${CPU40XX_LIB_TARGET_NAME}_test
    SOURCES
        CMakeLists.txt
        test/assembler_tests.cpp
        test/cpu4004_test_fixture.hpp
        test/instructions_tests.cpp
        test/programs_tests.cpp
    LIBRARIES
        ${CPU40XX_LIB_TARGET_NAME}
)

add_custom_command(
    TARGET ${CPU40XX_LIB_TARGET_NAME}_test
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/test_files/. ${CMAKE_CURRENT_BINARY_DIR}/test_files/cpu40xx
)
