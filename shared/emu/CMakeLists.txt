set(EMU_LIB_TARGET_NAME emu)

find_package(SDL3 REQUIRED)

add_library(${EMU_LIB_TARGET_NAME} STATIC
    CMakeLists.txt
    src/application.cpp
    src/renderer_2d.cpp
    src/include/emu/renderer_2d.hpp
    src/include/emu/application.hpp
)

set_target_warnings(${EMU_LIB_TARGET_NAME})

target_compile_features(${EMU_LIB_TARGET_NAME} PUBLIC cxx_std_23)

target_include_directories(${EMU_LIB_TARGET_NAME} PUBLIC
    src/include
)

target_link_libraries(${EMU_LIB_TARGET_NAME} PRIVATE
    SDL3::SDL3-static
    utils
)
