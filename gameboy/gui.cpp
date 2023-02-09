#include "gui.hpp"
#include "gameboy.hpp"

#include <glad/gl.h>
#include <glw/glw.hpp>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <tinyfiledialogs.h>

namespace GUI {

    static GLFWwindow* s_window = nullptr;
    static glw::Texture* s_tileDataTexture = nullptr;
    static glw::Framebuffer* s_tileMap0FB = nullptr;
    static glw::Framebuffer* s_tileMap1FB = nullptr;

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
                PPU::TILE_DATA_WIDTH, PPU::TILE_DATA_HEIGHT
            }
        };

        s_tileMap0FB = new glw::Framebuffer{
            glw::Framebuffer::Properties{
                256, 256, 1, {
                    glw::TextureSpecification{
                        glw::TextureFormat::RGBA8,
                        glw::TextureFilter::Nearest,
                        glw::TextureFilter::Nearest,
                        glw::TextureWrapMode::Clamp
                    }
                }
            }
        };

        s_tileMap1FB = new glw::Framebuffer{
            glw::Framebuffer::Properties{
                256, 256, 1, {
                    glw::TextureSpecification{
                        glw::TextureFormat::RGBA8,
                        glw::TextureFilter::Nearest,
                        glw::TextureFilter::Nearest,
                        glw::TextureWrapMode::Clamp
                    }
                }
            }
        };
    }

    void shutdown()
    {
        delete s_tileDataTexture;
        s_tileDataTexture = nullptr;

        delete s_tileMap0FB;
        s_tileMap0FB = nullptr;
        delete s_tileMap1FB;
        s_tileMap1FB = nullptr;

        s_window = nullptr;

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    static void drawTextureWindow(const glw::Texture& texture, bool flip, const char* title)
    {
        ImVec2 imageSize = { texture.getProperties().width * 2.f, texture.getProperties().height * 2.f};
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0, 0 });
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
        ImGui::SetNextWindowSize({ imageSize.x, imageSize.y + 8 }, ImGuiCond_Always);
        if (ImGui::Begin(title, nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize))
            ImGui::Image((ImTextureID)texture.getRendererID(), imageSize,
                flip ? ImVec2{ 0, 1 } : ImVec2{ 0, 0 }, flip ? ImVec2{ 1, 0 } : ImVec2{ 1, 1 });
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

        auto& ppu = gb.getPPU();
        auto pixels = ppu.getTileDataPixels();
        s_tileDataTexture->setData(pixels.data(), pixels.size() * sizeof(u32));
        drawTextureWindow(*s_tileDataTexture, false, "Tile Data");

        {glw::Renderer::beginFrame(s_tileMap0FB);
        auto tiles = ppu.getTileMap0();
        s_tileDataTexture->bind(0);
        for (u16 y = 0; y < 32; y++)
            for (u16 x = 0; x < 32; x++) {
                u16 index = y * 32 + x;
                u16 tileSigned = 256 + (s8)tiles[index];
                u16 tileUnsigned = tiles[index];
                u16 tile = ppu.getTileDataAddressingMode() ? tileUnsigned : tileSigned;
                u16 xpos = tile % 16;
                u16 ypos = tile / 16;
                constexpr float tileSize = 8;
                constexpr float tileMapHalfSize = 128;
                float left = (float)(x * tileSize) / tileMapHalfSize - 1.f;
                float right = (float)(x * tileSize + tileSize) / tileMapHalfSize - 1.f;
                float top = ((float)(y * tileSize) / tileMapHalfSize - 1.f);
                float bottom = ((float)(y * tileSize + tileSize) / tileMapHalfSize - 1.f);
                float u0 = (float)(xpos * tileSize) / PPU::TILE_DATA_WIDTH;
                float v0 = (float)(ypos * tileSize) / PPU::TILE_DATA_HEIGHT;
                float u1 = (float)(xpos * tileSize + tileSize) / PPU::TILE_DATA_WIDTH;
                float v1 = (float)(ypos * tileSize + tileSize) / PPU::TILE_DATA_HEIGHT;
                glw::Renderer::renderTexture(left, top, right, bottom, u0, v0, u1, v1);
            }
        glw::Renderer::endFrame();
        drawTextureWindow(s_tileMap0FB->getAttachments()[0], false, "Tile Map 0");}

        {glw::Renderer::beginFrame(s_tileMap1FB);
        auto tiles = ppu.getTileMap1();
        s_tileDataTexture->bind(0);
        for (u16 y = 0; y < 32; y++)
            for (u16 x = 0; x < 32; x++) {
                u16 index = y * 32 + x;
                u16 tileSigned = 256 + (s8)tiles[index];
                u16 tileUnsigned = tiles[index];
                u16 tile = ppu.getTileDataAddressingMode() ? tileUnsigned : tileSigned;
                u16 xpos = tile % 16;
                u16 ypos = tile / 16;
                constexpr float tileSize = 8;
                constexpr float tileMapHalfSize = 128;
                float left = (float)(x * tileSize) / tileMapHalfSize - 1.f;
                float right = (float)(x * tileSize + tileSize) / tileMapHalfSize - 1.f;
                float top = ((float)(y * tileSize) / tileMapHalfSize - 1.f);
                float bottom = ((float)(y * tileSize + tileSize) / tileMapHalfSize - 1.f);
                float u0 = (float)(xpos * tileSize) / PPU::TILE_DATA_WIDTH;
                float v0 = (float)(ypos * tileSize) / PPU::TILE_DATA_HEIGHT;
                float u1 = (float)(xpos * tileSize + tileSize) / PPU::TILE_DATA_WIDTH;
                float v1 = (float)(ypos * tileSize + tileSize) / PPU::TILE_DATA_HEIGHT;
                glw::Renderer::renderTexture(left, top, right, bottom, u0, v0, u1, v1);
            }
        glw::Renderer::endFrame();
        drawTextureWindow(s_tileMap1FB->getAttachments()[0], false, "Tile Map 1"); }

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
