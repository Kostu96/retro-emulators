#include "shared/source/application.hpp"

#include <glad/gl.h>
#include <glw/glw.hpp>
#include <GLFW/glfw3.h>

#include <iostream>

static void glfwErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW error " << error << ": " << description << '\n';
}

Application::Application(const Description& desc)
{
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit()) {
        std::cerr << "GLFW init failed!\n";
        std::terminate();
    }

    m_viewportX = m_viewportY= desc.border;
    m_viewportWidth = desc.rendererWidth * desc.scale;
    m_viewportHeight = desc.rendererHeight * desc.scale;
    const int WINDOW_WIDTH = m_viewportWidth + 2 * desc.border;
    const int WINDOW_HEIGHT = m_viewportHeight + 2 * desc.border;

    glfwWindowHint(GLFW_RESIZABLE, 0);
    m_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, desc.windowTitle, nullptr, nullptr);
    if (!m_window) {
        std::cerr << "GLFW window creation failed!\n";
        std::terminate();
    }
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);

    glw::init(glfwGetProcAddress);
    glw::Renderer::init();
    glClearColor(0.2f, 0.2f, 0.2f, 1.f);

    m_screenTexture = std::make_unique<glw::Texture>(glw::Texture::Properties{
        glw::TextureSpecification{
            glw::TextureFormat::RGBA8,
            glw::TextureFilter::Nearest,
            glw::TextureFilter::Nearest,
            glw::TextureWrapMode::Clamp
        },
        desc.rendererWidth, desc.rendererHeight
    });

    m_isRunning = true;
}

Application::~Application()
{
    glw::Renderer::shutdown();
    glfwTerminate();
}

void Application::run()
{
    while (!glfwWindowShouldClose(m_window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glw::Renderer::beginFrame();
        glViewport(m_viewportX, m_viewportY, m_viewportWidth, m_viewportHeight);
        //auto pixels = pet->getScreenPixels();
        //m_screenTexture->setData(pixels.data(), pixels.size() * sizeof(u32));
        m_screenTexture->bind(0);
        glw::Renderer::renderTexture(-1.f, 1.f, 1.f, -1.f, 0.f, 0.f, 1.f, 1.f);
        glw::Renderer::endFrame();
        
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
    m_isRunning = false;
}
