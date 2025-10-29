cmake_minimum_required(VERSION 3.22)
project(emulators LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

function(set_target_warnings TARGET_NAME)
    option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" ON)
    set(MSVC_WARNINGS
        /W4          # Baseline reasonable warnings
        /w14242      # 'identifier': conversion from 'type1' to 'type1', possible loss of data
        /w14254      # 'operator': conversion from 'type1:field_bits' to 'type2:field_bits', possible loss of data
        /w14263      # 'function': member function does not override any base class virtual member function
        /w14265      # 'classname': class has virtual functions, but destructor is not virtual instances of this class may not be destructed correctly
        /w14287      # 'operator': unsigned/negative constant mismatch
        /we4289      # nonstandard extension used: 'variable': loop control variable declared in the for-loop is used outside the for-loop scope
        /w14296      # 'operator': expression is always 'boolean_value'
        /w14311      # 'variable': pointer truncation from 'type1' to 'type2'
        /w14545      # expression before comma evaluates to a function which is missing an argument list
        /w14546      # function call before comma missing argument list
        /w14547      # 'operator': operator before comma has no effect; expected operator with side-effect
        /w14549      # 'operator': operator before comma has no effect; did you intend 'operator'?
        /w14555      # expression has no effect; expected expression with side- effect
        /w14619      # pragma warning: there is no warning number 'number'
        /w14640      # Enable warning on thread un-safe static member initialization
        /w14826      # Conversion from 'type1' to 'type_2' is sign-extended. This may cause unexpected runtime behavior.
        /w14905      # wide string literal cast to 'LPSTR'
        /w14906      # string literal cast to 'LPWSTR'
        /w14928      # illegal copy-initialization; more than one user-defined conversion has been implicitly applied
        /permissive- # standards conformance mode for MSVC compiler.
        /wd4201
    )
    set(CLANG_WARNINGS
        -Wall
        -Wextra                # reasonable and standard
        -Wshadow               # warn the user if a variable declaration shadows one from a parent context
        -Wnon-virtual-dtor     # warn the user if a class with virtual functions has a non-virtual destructor. This helps  catch hard to track down memory errors
        -Wold-style-cast       # warn for c-style casts
        -Wcast-align           # warn for potential performance problem casts
        -Wunused               # warn on anything being unused
        -Woverloaded-virtual   # warn if you overload (not override) a virtual function
        -Wpedantic             # warn if non-standard C++ is used
        -Wconversion           # warn on type conversions that may lose data
        -Wsign-conversion      # warn on sign conversions
        -Wnull-dereference     # warn if a null dereference is detected
        -Wdouble-promotion     # warn if float is implicit promoted to double
        -Wformat=2             # warn on security issues around functions that format output (ie printf)
        -Wimplicit-fallthrough # warn on statements that fallthrough without an explicit annotation
        -Wno-unknown-pragmas
    )
    if(WARNINGS_AS_ERRORS)
        set(CLANG_WARNINGS ${CLANG_WARNINGS} -Werror)
        set(MSVC_WARNINGS ${MSVC_WARNINGS} /WX)
    endif()
    set(GCC_WARNINGS
        ${CLANG_WARNINGS}
        -Wmisleading-indentation # warn if indentation implies blocks where blocks do not exist
        -Wduplicated-cond        # warn if if / else chain has duplicated conditions
        -Wduplicated-branches    # warn if if / else branches have duplicated code
        -Wlogical-op             # warn about logical operations being used where bitwise were probably wanted
        -Wuseless-cast           # warn if you perform a cast to the same type
    )
    if(MSVC)
        set(TARGET_WARNINGS ${MSVC_WARNINGS})
    elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
        set(TARGET_WARNINGS ${CLANG_WARNINGS})
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(TARGET_WARNINGS ${GCC_WARNINGS})
    else()
        message(AUTHOR_WARNING "No compiler warnings set for '${CMAKE_CXX_COMPILER_ID}' compiler.")
    endif()
    target_compile_options(${TARGET_NAME} PRIVATE ${TARGET_WARNINGS})
endfunction()

function(add_device_library)
    cmake_parse_arguments(PARSED "" "TARGET_NAME" "SOURCES;INCLUDE_DIRS;LIBRARIES" ${ARGN})
    add_library(${PARSED_TARGET_NAME} STATIC ${PARSED_SOURCES})
    set_target_warnings(${PARSED_TARGET_NAME})
    target_include_directories(${PARSED_TARGET_NAME} PUBLIC ${PARSED_INCLUDE_DIRS})
    target_link_libraries(${PARSED_TARGET_NAME} PUBLIC ${PARSED_LIBRARIES})
endfunction()

function(add_device_test)
    cmake_parse_arguments(PARSED "" "TARGET_NAME" "SOURCES;LIBRARIES" ${ARGN})
    add_executable(${PARSED_TARGET_NAME} ${PARSED_SOURCES})
    set_target_warnings(${PARSED_TARGET_NAME})
    target_link_libraries(${PARSED_TARGET_NAME} PUBLIC GTest::gtest GTest::gtest_main ${PARSED_LIBRARIES})
    gtest_discover_tests(${PARSED_TARGET_NAME})
endfunction()

enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

add_subdirectory(shared/utils)

add_subdirectory(shared/cpu40xx)
add_subdirectory(shared/cpu6502)
add_subdirectory(shared/cpu8008)
add_subdirectory(shared/cpu8080)

set_target_properties(
    cpu40xx
    cpu40xx_test
    cpu6502
    cpu6502_test
    cpu8008
    cpu8080
    cpu8080_test
    PROPERTIES
    FOLDER devices
)

#add_subdirectory(third_party)

