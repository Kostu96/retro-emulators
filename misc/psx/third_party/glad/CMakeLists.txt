cmake_minimum_required(VERSION 3.20)

project(glad-proj LANGUAGES C)
set(GLAD_TARGET_NAME glad)

add_library(${GLAD_TARGET_NAME} STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c
    ${CMAKE_CURRENT_SOURCE_DIR}/include/glad/glad.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/KHR/khrplatform.h
)

target_include_directories(${GLAD_TARGET_NAME} SYSTEM PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
