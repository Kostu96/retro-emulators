#pragma once
#include <memory>
#include <span>

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
        bool hasMenuBar;
    };

    Application(const Description& desc);
    virtual ~Application();

    void run();
    void exit();
    bool isRunning() const { return m_isRunning; }

    virtual std::span<const unsigned int> getScreenPixels() const = 0;
    virtual void onImGUIRender() {}

    virtual void onKeyCallback(int /*key*/, int /*action*/, int /*mods*/) {}
    virtual void onTextCallback(unsigned int /*codepoint*/) {}

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
