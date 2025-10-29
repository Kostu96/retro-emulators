set(CPU40XX_LIB_TARGET_NAME cpu40xx)

add_device_library(TARGET_NAME ${CPU40XX_LIB_TARGET_NAME}
    SOURCES
        CMakeLists.txt
        src/asm40xx.cpp
        src/asm40xx.hpp
        src/cpu40xx.cpp
        src/parser40xx.cpp
        src/parser40xx.hpp
        src/scanner40xx.cpp
        src/scanner40xx.hpp
        src/include/cpu40xx/cpu40xx.hpp
    INCLUDE_DIRS
        src/include
    LIBRARIES
        utils
)

add_device_test(TARGET_NAME ${CPU40XX_LIB_TARGET_NAME}_test
    SOURCES
        CMakeLists.txt
        test/instructions_tests.cpp
    LIBRARIES
        ${CPU40XX_LIB_TARGET_NAME}
)
