set(CPU8008_LIB_TARGET_NAME cpu8008)

add_device_library(
    TARGET_NAME ${CPU8008_LIB_TARGET_NAME}
    SOURCES
        CMakeLists.txt
        src/asm8008.cpp
        src/asm8008.hpp
        src/cpu8008.cpp
        src/scanner8008.cpp
        src/scanner8008.hpp
        src/include/cpu8008/cpu8008.hpp
    INCLUDE_DIRS
        src/include
    LIBRARIES
        utils
)
