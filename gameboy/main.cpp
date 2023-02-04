#include "cpu8080.hpp"
#include "cartridge.hpp"
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

    Cartridge cart;
    cart.loadFromFile("C:/Users/Konstanty/Desktop/retro-extras/programs/gameboy/blargg_test.gb");

    u8 vram[0x2000];
    u8 wram[0x2000];
    u8 hram[0x80];

    const AddressRange ROM_RANGE{    0x0000, 0x7FFF };
    const AddressRange VRAM_RANGE{   0x8000, 0x9FFF };
    const AddressRange WRAM_RANGE{   0xC000, 0xDFFF };
    const AddressRange TIMERS_RANGE{ 0xFF04, 0xFF07 };
    const AddressRange APU1_RANGE{   0xFF10, 0xFF14 };
    const AddressRange APU2_RANGE{   0xFF24, 0xFF26 };
    const AddressRange PPU_RANGE{    0xFF40, 0xFF4B };
    const AddressRange HRAM_RANGE{   0xFF80, 0xFFFF };

    bool mapBootloader = true;

    cpu.mapReadMemoryCallback([&](u16 address)
        {
            u16 offset;
            if (ROM_RANGE.contains(address, offset))
            {
                if (mapBootloader && offset < 0x100) return bootloader[offset];
                return cart.load8(offset);
            }

            if (WRAM_RANGE.contains(address, offset))
                return wram[offset];

            if (PPU_RANGE.contains(address, offset))
                return u8{0x90}; // TODO: graphics
            
            if (HRAM_RANGE.contains(address, offset))
                return hram[offset];

            assert(false && "Unhandled read");
            return u8{};
        });

    cpu.mapWriteMemoryCallback([&](u16 address, u8 data)
        {
            u16 offset;
            if (VRAM_RANGE.contains(address, offset)) {
                vram[offset] = data;
                return;
            }

            if (WRAM_RANGE.contains(address, offset)) {
                wram[offset] = data;
                return;
            }

            if (TIMERS_RANGE.contains(address, offset)) {
                // TODO: timers
                return;
            }

            if (APU1_RANGE.contains(address, offset)) {
                // TODO: sound
                return;
            }

            if (APU2_RANGE.contains(address, offset)) {
                // TODO: sound
                return;
            }

            if (PPU_RANGE.contains(address, offset)) {
                // TODO: graphics
                return;
            }

            if (HRAM_RANGE.contains(address, offset)) {
                hram[offset] = data;
                return;
            }

            if (address == 0xFF0F)
            {
                // TODO: interrupts
                return;
            }

            if (address == 0xFF50)
            {
                if (mapBootloader) mapBootloader = false;
                return;
            }

            assert(false && "Unhandled write");
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
