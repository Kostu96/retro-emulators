#include "gui.hpp"
#include "gameboy.hpp"

#include "shared/source/application.hpp"

#include <glw/glw.hpp>
#include <imgui.h>
#include <tinyfiledialogs.h>

namespace GUI {

    static Application* s_app = nullptr;
    static glw::Texture* s_tileDataTexture = nullptr;
    static glw::Framebuffer* s_tileMap0FB = nullptr;
    static glw::Framebuffer* s_tileMap1FB = nullptr;
    static bool s_showTileData = false;
    static bool s_showMap0 = false;
    static bool s_showMap1 = false;

    void init(Application* app)
    {       
        s_app = app;

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

        s_app = nullptr;
    }

    static void drawTextureWindow(const glw::Texture& texture, float scale, const char* title, bool& show)
    {
        ImVec2 imageSize = { texture.getProperties().width * scale, texture.getProperties().height * scale };
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0, 0 });
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
        ImGui::SetNextWindowSize({ imageSize.x, imageSize.y + 8 }, ImGuiCond_Always);
        if (ImGui::Begin(title, &show, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize))
            ImGui::Image((ImTextureID)texture.getRendererID(), imageSize, ImVec2{ 0, 0 }, ImVec2{ 1, 1 });
        ImGui::End();
        ImGui::PopStyleVar(2);
    }

    static void drawTileMapWindow(const glw::Framebuffer* fb, const std::span<u8>& tiles, bool addrMode, const char* title, bool& show)
    {
        glw::Renderer::beginFrame(fb);
        s_tileDataTexture->bind(0);
        for (u16 y = 0; y < 32; y++)
            for (u16 x = 0; x < 32; x++) {
                u16 index = y * 32 + x;
                u16 tile = addrMode ? tiles[index] : 256 + (s8)tiles[index];
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
        drawTextureWindow(fb->getAttachments()[0], 2.f, title, show);
    }

    void update(Gameboy& gb)
    {
        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Load cartridge...")) {
                static const char* filters[1] = { "*.gb" };
                char* filename = tinyfd_openFileDialog("Load cartridge", nullptr, 1, filters, nullptr, 0);
                if (filename) {
                    gb.loadCartridge(filename);
                    gb.reset();
                }
            }
            if (ImGui::MenuItem("Reset")) gb.reset();
            if (ImGui::MenuItem("Exit", "Alt+F4")) s_app->exit();

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Debug"))
        {
            ImGui::MenuItem("Show Tile Data", nullptr, &s_showTileData);
            ImGui::MenuItem("Show Tile Map 0", nullptr, &s_showMap0);
            ImGui::MenuItem("Show Tile Map 1", nullptr, &s_showMap1);

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();

        auto& ppu = gb.getPPU();
        if (s_showTileData || s_showMap0 || s_showMap1) {
            auto pixels = ppu.getTileDataPixels();
            s_tileDataTexture->setData(pixels.data(), pixels.size() * sizeof(u32));
        }
        if (s_showTileData)
            drawTextureWindow(*s_tileDataTexture, 3.f, "Tile Data", s_showTileData);
        if (s_showMap0)
            drawTileMapWindow(s_tileMap0FB, ppu.getTileMap0(), ppu.getTileDataAddressingMode(), "Tile Map 0", s_showMap0);
        if (s_showMap1)
            drawTileMapWindow(s_tileMap1FB, ppu.getTileMap1(), ppu.getTileDataAddressingMode(), "Tile Map 1", s_showMap1);
    }

} // namespace GUI
