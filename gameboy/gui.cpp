#include "gui.hpp"
#include "ppu.hpp"

#include <glw/texture.hpp>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace GUI {

    static GLFWwindow* s_window = nullptr;

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
    }

    void shutdown()
    {
        s_window = nullptr;

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    static void drawTextureWindow(const glw::Texture* texture, const char* title)
    {
        ImVec2 imageSize = { texture->getProperties().width * 2.f, texture->getProperties().height * 2.f };
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0, 0 });
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
        ImGui::SetNextWindowSize({ imageSize.x, imageSize.y + 8 }, ImGuiCond_Always);
        if (ImGui::Begin(title, nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize))
            ImGui::Image((ImTextureID)texture->getRendererID(), imageSize/*, { 0, 1 }, { 1, 0 }*/);
        ImGui::End();
        ImGui::PopStyleVar(2);
    }

    void update(const PPU& ppu)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit", "Alt+F4")) glfwSetWindowShouldClose(s_window, true);

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();

        drawTextureWindow(ppu.getTileDataTexture(), "Tile Data");

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
