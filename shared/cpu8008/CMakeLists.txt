set(CPU8008_LIB_TARGET_NAME cpu8008)

add_library(${CPU8008_LIB_TARGET_NAME} STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/src/asm8008.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/asm8008.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu8008.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/scanner8008.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/scanner8008.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/cpu8008/cpu8008.hpp
)

target_include_directories(${CPU8008_LIB_TARGET_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
)

target_link_libraries(${CPU8008_LIB_TARGET_NAME} PUBLIC
    utils
)
