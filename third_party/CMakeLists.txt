set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(glw)

#set(IMGUI_SOURCES
#    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.h
#    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.h
#    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3_loader.h
#    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.h
#    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_internal.h
#    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_rectpack.h
#    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_textedit.h
#    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_truetype.h
#)

#add_library(imgui STATIC ${IMGUI_SOURCES})

#target_include_directories(imgui SYSTEM PUBLIC
#    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
#)

set_target_properties(
    glw
#    imgui
    PROPERTIES
    FOLDER third_party
)
