set(EMU_LIB_TARGET_NAME emu)

find_package(SDL3 REQUIRED)

add_library(${EMU_LIB_TARGET_NAME} STATIC
    CMakeLists.txt
    src/application.cpp
    src/include/emu/application.hpp
)

set_target_warnings(${EMU_LIB_TARGET_NAME})

target_include_directories(${EMU_LIB_TARGET_NAME} PUBLIC
    src/include
)

target_link_libraries(${EMU_LIB_TARGET_NAME} PRIVATE
    SDL3::SDL3-static
    utils
)
