#include "cpu8080.hpp"
#include "cartridge.hpp"
#include "ppu.hpp"
#include "../shared/memory_map.hpp"

#include <glad/gl.h>
#include <glw/glw.hpp>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <iostream>
#include <fstream>
#include <thread>

constexpr u16 FRAME_WIDTH = 160;
constexpr u16 FRAME_HEIGHT = 144;
constexpr u16 SCALE = 3;
constexpr u16 BORDER_SIZE = 12;
constexpr u16 IMGUI_MENU_BAR_HEIGHT = 6;
constexpr u16 WINDOW_WIDTH = FRAME_WIDTH * SCALE + 2 * BORDER_SIZE;
constexpr u16 WINDOW_HEIGHT = FRAME_HEIGHT * SCALE + 2 * BORDER_SIZE;

constexpr u16 VRAM_SIZE = 0x2000;

constexpr u16 TILES_FRAME_WIDTH = 16 * 8;
constexpr u16 TILES_FRAME_HEIGHT = 24 * 8;
constexpr u16 TILEMAP_FRAME_SIZE = 256;

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

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT + IMGUI_MENU_BAR_HEIGHT, "Game Boy", nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW window creation failed!\n";
        std::terminate();
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    glw::init(glfwGetProcAddress);
    glw::Renderer::init();

    glw::Framebuffer* tileDataFBO = new glw::Framebuffer{
        glw::Framebuffer::Properties{ TILES_FRAME_WIDTH, TILES_FRAME_HEIGHT, 1, {
                glw::TextureSpecification{
                    glw::TextureFormat::RGBA8,
                    glw::TextureFilter::Nearest,
                    glw::TextureFilter::Nearest,
                    glw::TextureWrapMode::Clamp
                }
            }} };

    glw::Framebuffer* tileMap0FBO = new glw::Framebuffer{
        glw::Framebuffer::Properties{ TILEMAP_FRAME_SIZE, TILEMAP_FRAME_SIZE, 1, {
                glw::TextureSpecification{
                    glw::TextureFormat::RGBA8,
                    glw::TextureFilter::Nearest,
                    glw::TextureFilter::Nearest,
                    glw::TextureWrapMode::Repeat
                }
            }} };

    glw::Framebuffer* tileMap1FBO = new glw::Framebuffer{
        glw::Framebuffer::Properties{ TILEMAP_FRAME_SIZE, TILEMAP_FRAME_SIZE, 1, {
                glw::TextureSpecification{
                    glw::TextureFormat::RGBA8,
                    glw::TextureFilter::Nearest,
                    glw::TextureFilter::Nearest,
                    glw::TextureWrapMode::Repeat
                }
            }} };

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");

    CPU8080 cpu{ CPU8080::Mode::GameBoy };
    PPU ppu;

    u8 bootloader[256];
    std::ifstream file{ argv[1], std::ios_base::binary};
    if (!file.is_open()) {
        std::cerr << "Cannot open bootloader file!\n";
        std::terminate();
    }
    file.read((char*)bootloader, 256);
    file.close();
    bootloader[0x0003] = 0x18;
    bootloader[0x0004] = 0x07;

    Cartridge cart;
    cart.loadFromFile("C:/Users/Konstanty/Desktop/retro-extras/programs/gameboy/tetris.gb");
    //cart.loadFromFile("C:/Users/Konstanty/Desktop/retro-extras/programs/gameboy/01-special.gb");
    //cart.loadFromFile("C:/Users/Konstanty/Desktop/retro-extras/programs/gameboy/04-op r,imm.gb");
    //cart.loadFromFile("C:/Users/Konstanty/Desktop/retro-extras/programs/gameboy/06-ld r,r.gb");
    //cart.loadFromFile("C:/Users/Konstanty/Desktop/retro-extras/programs/gameboy/blargg_test.gb");
    //cart.loadFromFile("C:/Users/Konstanty/Desktop/retro-extras/programs/gameboy/dmg-acid2.gb");

    u8* vram = new u8[VRAM_SIZE];
    u8 wram[0x2000];
    u8 hram[0x80];

    const AddressRange CART_RANGE{   0x0000, 0x7FFF };
    const AddressRange VRAM_RANGE{   0x8000, 0x9FFF };
    const AddressRange WRAM_RANGE{   0xC000, 0xDFFF };
    const AddressRange OAM_RANGE{    0xFE00, 0xFE9F };
    const AddressRange SERIAL_RANGE{ 0xFF00, 0xFF02 };
    const AddressRange TIMERS_RANGE{ 0xFF04, 0xFF07 };
    const AddressRange APU1_RANGE{   0xFF10, 0xFF14 };
    const AddressRange APU2_RANGE{   0xFF24, 0xFF26 };
    const AddressRange PPU_RANGE{    0xFF40, 0xFF4B };
    const AddressRange HRAM_RANGE{   0xFF80, 0xFFFF };

    bool mapBootloader = true;
    bool redrawTileData = false;
    bool redrawTileMap0 = false;
    bool redrawTileMap1 = false;

    cpu.mapReadMemoryCallback([&](u16 address)
        {
            u16 offset;
            if (CART_RANGE.contains(address, offset))
            {
                if (mapBootloader && offset < 0x100) return bootloader[offset];
                return cart.load8(offset);
            }

            if (WRAM_RANGE.contains(address, offset))
                return wram[offset];

            if (PPU_RANGE.contains(address, offset))
                return ppu.load8(offset);
            
            if (HRAM_RANGE.contains(address, offset))
                return hram[offset];

            assert(false && "Unhandled read");
            return u8{};
        });

    cpu.mapWriteMemoryCallback([&](u16 address, u8 data)
        {
            u16 offset;
            if (CART_RANGE.contains(address, offset)) {
                cart.store8(offset, data);
                return;
            }

            if (VRAM_RANGE.contains(address, offset)) {
                redrawTileData = offset < 0x1800;
                redrawTileMap0 = offset >= 0x1800 && offset < 0x1C00;
                redrawTileMap1 = offset >= 0x1C00 && offset < 0x2000;
                vram[offset] = data;
                return;
            }

            if (WRAM_RANGE.contains(address, offset)) {
                wram[offset] = data;
                return;
            }

            if (OAM_RANGE.contains(address, offset)) {
                // TODO: sprites
                return;
            }

            if (SERIAL_RANGE.contains(address, offset)) {
                if (offset == 0x0)
                    std::cout << data;
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
                ppu.store8(offset, data);
                redrawTileMap0 = (offset == 0 && data & 0x10);
                redrawTileMap1 = (offset == 0 && data & 0x10);
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
    ppu.reset();

    u32 colors[4]{
        0xEEEEEEFF,
        0x999999FF,
        0x444444FF,
        0x000000FF,
    };

    glClearColor(0.f, 0.f, 0.f, 0.f);
    while (!glfwWindowShouldClose(window))
    {
        int repeats = 1;
        while (repeats--)
        {
            if (mapBootloader && cpu.getPC() == 0x0003)
            {
                // bootloader routine to clear the VRAM
                std::memset(vram, 0, VRAM_SIZE);
                redrawTileData = true;
                redrawTileMap0 = true;
                redrawTileMap1 = true;
            }

            cpu.clock();
            ppu.clock();
        }

        glClear(GL_COLOR_BUFFER_BIT);

        if (redrawTileData)
        {
            redrawTileData = false;
            tileDataFBO->bind();
            glViewport(0, 0, TILES_FRAME_WIDTH, TILES_FRAME_HEIGHT);
            glw::Renderer::beginFrame();
            u16 address = 0;
            for (u16 y = 0; y < 24; y++) {
                for (u16 x = 0; x < 16; x++) {
                    for (u8 tileY = 0; tileY < 16; tileY += 2) {
                        u8 b1 = vram[address + tileY];
                        u8 b2 = vram[address + tileY + 1];
                        for (s8 bit = 7; bit >= 0; bit--) {
                            u8 color = (((b2 >> bit) & 1) << 1) | ((b1 >> bit) & 1);
                            glw::Renderer::renderPoint(x * 8 + 7 - bit, y * 8 + tileY / 2 + 1, colors[color]);
                        }
                    }

                    address += 16;
                }
            }
            glw::Renderer::endFrame();
            tileDataFBO->unbind();
        }

        if (redrawTileMap0)
        {
            redrawTileMap0 = false;
            tileMap0FBO->bind();
            glViewport(0, 0, TILEMAP_FRAME_SIZE, TILEMAP_FRAME_SIZE);
            glw::Renderer::beginFrame();
            tileDataFBO->getAttachments()[0].bind(0);
            u16 address = 0x1800;
            for (u16 y = 0; y < 32; y++) {
                for (u16 x = 0; x < 32; x++) {
                    u16 index = vram[address];
                    if (ppu.getTileDataArea() == 0 && index < 128) index += 256;
                    u16 ypos = index / 16;
                    u16 xpos = index % 16;
                    float left = (float)(x * 8) / (256.f * 0.5f) - 1.f;
                    float right = (float)(x * 8 + 8) / (256.f * 0.5f) - 1.f;
                    float top = -((float)(y * 8) / (256.f * 0.5f) - 1.f);
                    float bottom = -((float)(y * 8 + 8) / (256.f * 0.5f) - 1.f);
                    float u0 = (float)(xpos * 8) / 128.f;
                    float v0 = 1.f - (float)(ypos * 8) / 192.f;
                    float u1 = (float)(xpos * 8 + 8) / 128.f;
                    float v1 = 1.f - (float)(ypos * 8 + 8) / 192.f;
                    glw::Renderer::renderTexture(left, top, right, bottom, u0, v0, u1, v1);
                    address++;
                }
            }
            glw::Renderer::endFrame();
            tileMap0FBO->unbind();
        }

        if (redrawTileMap1)
        {
            redrawTileMap1 = false;
            tileMap1FBO->bind();
            glViewport(0, 0, TILEMAP_FRAME_SIZE, TILEMAP_FRAME_SIZE);
            glw::Renderer::beginFrame();
            tileDataFBO->getAttachments()[0].bind(0);
            u16 address = 0x1C00;
            for (u16 y = 0; y < 32; y++) {
                for (u16 x = 0; x < 32; x++) {
                    u16 index = vram[address];
                    if (ppu.getTileDataArea() == 0 && index < 128) index += 256;
                    u16 ypos = index / 16;
                    u16 xpos = index % 16;
                    float left = (float)(x * 8) / (256.f * 0.5f) - 1.f;
                    float right = (float)(x * 8 + 8) / (256.f * 0.5f) - 1.f;
                    float top = -((float)(y * 8) / (256.f * 0.5f) - 1.f);
                    float bottom = -((float)(y * 8 + 8) / (256.f * 0.5f) - 1.f);
                    float u0 = (float)(xpos * 8) / 128.f;
                    float v0 = 1.f - (float)(ypos * 8) / 192.f;
                    float u1 = (float)(xpos * 8 + 8) / 128.f;
                    float v1 = 1.f - (float)(ypos * 8 + 8) / 192.f;
                    glw::Renderer::renderTexture(left, top, right, bottom, u0, v0, u1, v1);
                    address++;
                }
            }
            glw::Renderer::endFrame();
            tileMap1FBO->unbind();
        }

        glw::Renderer::beginFrame();
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        tileMap0FBO->getAttachments()[0].bind(0);
        float left = (float)BORDER_SIZE / (WINDOW_WIDTH * 0.5f) - 1.f;
        float right = (float)(BORDER_SIZE + FRAME_WIDTH * SCALE) / (WINDOW_WIDTH * 0.5f) - 1.f;
        float top = -((float)BORDER_SIZE / (WINDOW_HEIGHT * 0.5f) - 1.f);
        float bottom = -((float)(BORDER_SIZE + FRAME_HEIGHT * SCALE) / (WINDOW_HEIGHT * 0.5f) - 1.f);
        float u0 = (float)ppu.getSCX() / 256.f;
        float v0 = 1.f - (float)ppu.getSCY() / 256.f;
        float u1 = (float)(ppu.getSCX() + 160) / 256.f;
        float v1 = 1.f - (float)(ppu.getSCY() + 144) / 256.f;
        glw::Renderer::renderTexture(left, top, right, bottom, u0, v0, u1, v1);
        glw::Renderer::endFrame();

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

        ImGui::SetNextWindowSize({ TILES_FRAME_WIDTH * 2 + 16, TILES_FRAME_HEIGHT * 2 + 36 }, ImGuiCond_Always);
        if (ImGui::Begin("Tile Data", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize))
        {
            ImGui::Image((ImTextureID)tileDataFBO->getAttachments()[0].getRendererID(), { TILES_FRAME_WIDTH * 2, TILES_FRAME_HEIGHT * 2 }, { 0, 1 }, { 1, 0 });
        }
        ImGui::End();
        ImGui::SetNextWindowSize({ TILEMAP_FRAME_SIZE + 16, TILEMAP_FRAME_SIZE + 36 }, ImGuiCond_Always);
        if (ImGui::Begin("Tile Map 0", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize))
        {
            ImGui::Image((ImTextureID)tileMap0FBO->getAttachments()[0].getRendererID(), { TILEMAP_FRAME_SIZE, TILEMAP_FRAME_SIZE }, { 0, 1 }, { 1, 0 });
        }
        ImGui::End();
        ImGui::SetNextWindowSize({ TILEMAP_FRAME_SIZE + 16, TILEMAP_FRAME_SIZE + 36 }, ImGuiCond_Always);
        if (ImGui::Begin("Tile Map 1", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize))
        {
            ImGui::Image((ImTextureID)tileMap1FBO->getAttachments()[0].getRendererID(), { TILEMAP_FRAME_SIZE, TILEMAP_FRAME_SIZE }, { 0, 1 }, { 1, 0 });
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

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete[] vram;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    delete tileDataFBO;
    delete tileMap0FBO;
    delete tileMap1FBO;
    glw::Renderer::shutdown();

    glfwTerminate();
    return 0;
}
