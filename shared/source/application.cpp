#include "shared/source/application.hpp"
#include "shared/source/imgui/imgui_helper.hpp"

#include <glad/gl.h>
#include <glw/glw.hpp>
#include <GLFW/glfw3.h>

#include <iostream>

static void glfwErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW error " << error << ": " << description << '\n';
}

static void glfwKeyCallback(GLFWwindow* window, int key, int /*scancode*/, int action, int mods)
{
    auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    app->onKeyCallback(key, action, mods);
}

static void glfwTextCallback(GLFWwindow* window, unsigned int codepoint)
{
    auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    app->onTextCallback(codepoint);
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
    const int WINDOW_HEIGHT = m_viewportHeight + 2 * desc.border + (desc.hasMenuBar ? 20 : 0);

    glfwWindowHint(GLFW_RESIZABLE, 0);
    m_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, desc.windowTitle, nullptr, nullptr);
    if (!m_window) {
        std::cerr << "GLFW window creation failed!\n";
        std::terminate();
    }

    glfwSetWindowUserPointer(m_window, this);
    glfwSetKeyCallback(m_window, glfwKeyCallback);
    glfwSetCharCallback(m_window, glfwTextCallback);

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);

    glw::init(glfwGetProcAddress);
    glw::Renderer::init();
    glClearColor(0.2f, 0.2f, 0.2f, 1.f);
    imgui::init(m_window);

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
    imgui::shutdown();
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
        auto pixels = getScreenPixels();
        m_screenTexture->setData(pixels.data(), pixels.size() * sizeof(unsigned int));
        m_screenTexture->bind(0);
        glw::Renderer::renderTexture(-1.f, 1.f, 1.f, -1.f, 0.f, 0.f, 1.f, 1.f);
        glw::Renderer::endFrame();
        
        imgui::beginFrame();
        onImGUIRender();
        imgui::endFrame();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
    m_isRunning = false;
}

void Application::exit()
{
    glfwSetWindowShouldClose(m_window, true);
}
