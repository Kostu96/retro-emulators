#include "cpu.h"
#include "opengl/renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

static void GLAPIENTRY openGlErrorHandler(
    GLenum source,
    GLenum type,
    GLuint /*id*/,
    GLenum severity,
    GLsizei /*length*/,
    const GLchar* message,
    const void* /*userParam*/)
{
    const char* sourceStr = "";
    switch (source) {
    case GL_DEBUG_SOURCE_API: sourceStr = "Call to the OpenGL API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: sourceStr = "Call to a window-system API"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceStr = "Compiler for a shading language"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY: sourceStr = "Application associated with OpenGL"; break;
    case GL_DEBUG_SOURCE_APPLICATION: sourceStr = "User of this application"; break;
    case GL_DEBUG_SOURCE_OTHER: sourceStr = "Unknown"; break;
    }

    const char* typeStr = "";
    switch (type) {
    case GL_DEBUG_TYPE_ERROR: typeStr = "Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeStr = "Deprecated behavior"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: typeStr = "Undefined behavior"; break;
    case GL_DEBUG_TYPE_PORTABILITY: typeStr = "Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE: typeStr = "Performance"; break;
    case GL_DEBUG_TYPE_MARKER: typeStr = "Command stream annotation"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
    case GL_DEBUG_TYPE_POP_GROUP: typeStr = "User defined"; break;
    case GL_DEBUG_TYPE_OTHER: typeStr = "Unknown"; break;
    }

    std::cerr << "OpenGL debug | " << sourceStr << " | Type: " << typeStr << '\n' << message;
}

int main()
{
    if (!glfwInit()) {
        std::cerr << "GLFW init failed!\n";
        abort();
    }

    GLFWwindow* window = glfwCreateWindow(2 * 1024, 2 * 512, "PSX Emulator", nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW window creation failed!\n";
        abort();
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "GLAD loader failed!\n";
        abort();
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openGlErrorHandler, nullptr);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    Renderer renderer(window);
    CPU cpu(renderer);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        for (size_t i = 0; i < 5000000; ++i)
            cpu.runNextInstruction();
    }
    
    glfwTerminate();
    return 0;
}
