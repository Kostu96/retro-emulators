#include "cpu8080.hpp"
#include "../shared/memory_map.hpp"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <iostream>
#include <fstream>

constexpr u16 FRAME_WIDTH = 160;
constexpr u16 FRAME_HEIGHT = 144;
constexpr u16 SCALE = 2;
constexpr u16 BORDER_SIZE = 16;
constexpr u16 IMGUI_MENU_BAR_HEIGHT = 6;
constexpr u16 WINDOW_WIDTH = FRAME_WIDTH * SCALE + 2 * BORDER_SIZE;
constexpr u16 WINDOW_HEIGHT = FRAME_HEIGHT * SCALE + 2 * BORDER_SIZE + IMGUI_MENU_BAR_HEIGHT;

static void glfwErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW error " << error << ": " << description << '\n';
}

int main(int /*argc*/, char* argv[])
{
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit()) {
        std::cerr << "GLFW init failed!\n";
        std::terminate();
    }

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game Boy", nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW window creation failed!\n";
        std::terminate();
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");

    CPU8080 cpu{ CPU8080::Mode::GameBoy };

    u8 bootloader[256];
    std::ifstream file{ argv[1], std::ios_base::binary};
    if (!file.is_open()) {
        std::cerr << "Cannot open bootloader file!\n";
        std::terminate();
    }
    file.read((char*)bootloader, 256);
    file.close();

    u8 vram[0x2000];

    const AddressRange ROM_RANGE{  0x0000, 0x7FFF };
    const AddressRange VRAM_RANGE{ 0x8000, 0x9FFF };

    cpu.mapReadMemoryCallback([&bootloader](u16 address)
        {
            return bootloader[address];
        });

    cpu.mapWriteMemoryCallback([&](u16 address, u8 data)
        {
            u16 offset;
            if (VRAM_RANGE.contains(address, offset))
                vram[offset] = data;
        });

    cpu.reset();
    while (!glfwWindowShouldClose(window))
    {
        cpu.clock();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit", "Alt+F4")) glfwSetWindowShouldClose(window, true);

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
