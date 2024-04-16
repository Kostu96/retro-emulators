#pragma once
#include <memory>

namespace glw {
    class Texture;
}
struct GLFWwindow;

class Application
{
public:
    struct Description
    {
        const char* windowTitle;
        unsigned int rendererWidth;
        unsigned int rendererHeight;
        unsigned int scale;
        unsigned int border;
    };

    Application(const Description& desc);
    virtual ~Application();

    void run();
    bool isRunning() const { return m_isRunning; }

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
private:
    GLFWwindow* m_window = nullptr;
    std::unique_ptr<glw::Texture> m_screenTexture{};
    bool m_isRunning = false;
    int m_viewportX;
    int m_viewportY;
    int m_viewportWidth;
    int m_viewportHeight;
};
