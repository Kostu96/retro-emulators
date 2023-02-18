#include "gui.hpp"
#include "altair.hpp"

#include <glad/gl.h>
#include <glw/glw.hpp>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <tinyfiledialogs.h>

namespace GUI {

    static GLFWwindow* s_window = nullptr;
    static bool s_switches[16]{};
    static const char* s_middleLEDLabels[16] = {
        "A0", "A1", "A2",
        "A3", "A4", "A5",
        "A6", "A7", "A8",
        "A9", "A10", "A11",
        "A12", "A13", "A14",
        "A15",
    };
    static const char* s_switchLabels[16] = {
        "Switch0", "Switch1", "Switch2",
        "Switch3", "Switch4", "Switch5",
        "Switch6", "Switch7", "Switch8",
        "Switch9", "Switch10", "Switch11",
        "Switch12", "Switch13", "Switch14",
        "Switch15",
    };

    void init(GLFWwindow* window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        ImGui::StyleColorsDark();
        ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = { 0.4f, 0.4f, 0.4f, 1.f };
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

    static void MiddleLEDsGroup() {
        float frameHeight = ImGui::GetFrameHeight() * 1.2f;
        float radius = frameHeight * 0.50f;
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        for (int i = 15; i >= 0; i--) {
            ImVec2 p = ImGui::GetCursorScreenPos();

            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::InvisibleButton("##", { frameHeight, frameHeight / 1.5f });
            ImGui::PopItemFlag();

            if (i % 3 == 0) {
                ImGui::SameLine();
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                ImGui::InvisibleButton("##", { frameHeight / 2.f, frameHeight / 1.5f });
                ImGui::PopItemFlag();
            }

            float textHalfWidth = ImGui::CalcTextSize(s_middleLEDLabels[i]).x / 2.f;
            draw_list->AddText({ p.x + frameHeight / 2.f - textHalfWidth, p.y }, ImGui::GetColorU32(ImGuiCol_Text), s_middleLEDLabels[i]);

            if (i != 0) ImGui::SameLine();
        }

        for (int i = 15; i >= 0; i--) {
            ImVec2 p = ImGui::GetCursorScreenPos();

            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::InvisibleButton("##", { frameHeight, frameHeight * 2 });
            ImGui::PopItemFlag();

            if (i % 3 == 0) {
                ImGui::SameLine();
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                ImGui::InvisibleButton("##", { frameHeight / 2.f, frameHeight * 2 });
                ImGui::PopItemFlag();
            }

            draw_list->AddCircleFilled({ p.x + radius, p.y + radius }, radius, IM_COL32(0x6b, 0, 3, 255));

            if (i != 0) ImGui::SameLine();
        }
    }

    static void ToggleButtonsGroup(int count, const char** labels, bool* states) {
        float frameHeight = ImGui::GetFrameHeight() * 1.2f;
        float height = frameHeight * 1.8f;
        float radius = frameHeight * 0.50f;
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        for (int i = count - 1; i >= 0; i--) {
            ImVec2 p = ImGui::GetCursorScreenPos();

            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::InvisibleButton("##", ImVec2{ frameHeight, frameHeight / 1.5f });
            ImGui::PopItemFlag();

            if (i % 3 == 0) {
                ImGui::SameLine();
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                ImGui::InvisibleButton("##", ImVec2{ frameHeight / 2.f, frameHeight / 1.5f });
                ImGui::PopItemFlag();
            }

            float textHalfWidth = ImGui::CalcTextSize(labels[i] + 6).x / 2.f;
            draw_list->AddText({ p.x + frameHeight / 2.f - textHalfWidth, p.y }, ImGui::GetColorU32(ImGuiCol_Text), labels[i] + 6);

            if (i != 0) ImGui::SameLine();
        }

        for (int i = count - 1; i >= 0; i--) {
            ImVec2 p = ImGui::GetCursorScreenPos();

            if (ImGui::InvisibleButton(labels[i], ImVec2{ frameHeight, height }))
                states[i] = !states[i];
            
            if (i % 3 == 0) {
                ImGui::SameLine();
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                ImGui::InvisibleButton("##", ImVec2{ frameHeight / 2.f, height });
                ImGui::PopItemFlag();
            }

            ImU32 col_bg = ImGui::IsItemHovered() ?
                (states[i] ? IM_COL32(165, 211, 88, 255) : IM_COL32(198, 198, 198, 255)) :
                (states[i] ? IM_COL32(145, 211, 68, 255) : IM_COL32(218, 218, 218, 255));

            draw_list->AddRectFilled(p, ImVec2{ p.x + frameHeight, p.y + height }, col_bg, height * 0.5f);
            draw_list->AddCircleFilled(ImVec2{ p.x + radius, states[i] ? (p.y + height - radius) : (p.y + radius) }, radius - 1.5f, IM_COL32(255, 255, 255, 255));

            if (i != 0) ImGui::SameLine();
        }
    }

    void update(Altair& altair)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("File"))
        {
            /*if (ImGui::MenuItem("Load cartridge...")) {
                static const char* filters[1] = { "*.gb" };
                char* filename = tinyfd_openFileDialog("Load cartridge", nullptr, 1, filters, nullptr, 0);
                if (filename) {
                    gb.loadCartridge(filename);
                    gb.reset();
                }
            }*/
            if (ImGui::MenuItem("Reset")) altair.reset();
            if (ImGui::MenuItem("Exit", "Alt+F4")) glfwSetWindowShouldClose(s_window, true);

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();

        auto windowPos = ImGui::GetMainViewport()->Pos;
        ImGui::SetNextWindowPos({ windowPos.x, windowPos.y + ImGui::GetFrameHeight() }, ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size, ImGuiCond_Always);
        if (ImGui::Begin("Front Panel", nullptr, ImGuiWindowFlags_NoDecoration))
        {
            MiddleLEDsGroup();
            ToggleButtonsGroup(16, s_switchLabels, s_switches);
        }
        ImGui::End();

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
