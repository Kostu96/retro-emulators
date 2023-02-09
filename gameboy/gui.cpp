#include "gui.hpp"
#include "gameboy.hpp"

#include <glw/texture.hpp>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <tinyfiledialogs.h>

namespace GUI {

    static GLFWwindow* s_window = nullptr;
    static glw::Texture* s_tileDataTexture = nullptr;

    void init(GLFWwindow* window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 450");

        s_window = window;

        s_tileDataTexture = new glw::Texture{
            glw::Texture::Properties{
                glw::TextureSpecification{
                    glw::TextureFormat::RGBA8,
                    glw::TextureFilter::Nearest,
                    glw::TextureFilter::Nearest,
                    glw::TextureWrapMode::Clamp
                },
                Gameboy::getTileDataWidth(), Gameboy::getTileDataHeight()
            }
        };
    }

    void shutdown()
    {
        delete s_tileDataTexture;

        s_window = nullptr;

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    static void drawTextureWindow(std::span<u32> pixels, const char* title)
    {
        s_tileDataTexture->setData(pixels.data(), pixels.size() * sizeof(u32));
        ImVec2 imageSize = { Gameboy::getTileDataWidth() * 2.f, Gameboy::getTileDataHeight() * 2.f };
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0, 0 });
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
        ImGui::SetNextWindowSize({ imageSize.x, imageSize.y + 8 }, ImGuiCond_Always);
        if (ImGui::Begin(title, nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize))
            ImGui::Image((ImTextureID)s_tileDataTexture->getRendererID(), imageSize);
        ImGui::End();
        ImGui::PopStyleVar(2);
    }

    void update(Gameboy& gb)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Load cartridge...")) {
                char* filename = tinyfd_openFileDialog("Load cartridge", "", 0, nullptr, nullptr, 0);
                gb.loadCartridge(filename);
                gb.reset();
            }
            if (ImGui::MenuItem("Reset")) gb.reset();
            if (ImGui::MenuItem("Exit", "Alt+F4")) glfwSetWindowShouldClose(s_window, true);

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();

        drawTextureWindow(gb.getTileDataPixels(), "Tile Data");

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

} // namespace GUI
