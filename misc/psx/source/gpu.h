#pragma once
#include "type_helpers.h"
#include "opengl/renderer.h"

#include <cstdint>
#include <functional>

class GPU
{
public:
	GPU(Renderer& renderer) : m_renderer(renderer) {};

	uint32_t load32(uint32_t offset) const;
	void store32(uint32_t offset, uint32_t value);
private:
	enum class GP0Mode {
		Command,
		ImageLoad
	};

	class CommandBuffer
	{
	public:
		void clear() { m_size = 0; }
		void push(uint32_t command) { m_commands[m_size++] = command; } // TODO: check for overflow!
		uint32_t operator[](size_t i) const { return m_commands[i]; }
	private:
		uint32_t m_commands[12]{};
		uint8_t m_size = 0;
	};

	void gp0write(uint32_t value);
	void NOP(uint32_t instruction);
	void ClearCache(uint32_t instruction);
	void RenderOpaqueMonochromeQuad(uint32_t instruction);
	void RenderOpaqueTexturedQuadWithBlending(uint32_t instruction);
	void RenderOpaqueShadedTriangle(uint32_t instruction);
	void RenderOpaqueShadedQuad(uint32_t instruction);
	void CopyRectangleToVRAM(uint32_t instruction);
	void CopyRectangleFromVRAM(uint32_t instruction);
	void DrawMode(uint32_t instruction);
	void TextureWindow(uint32_t instruction);
	void SetDrawingAreaTL(uint32_t instruction);
	void SetDrawingAreaBR(uint32_t instruction);
	void SetDrawingOffset(uint32_t instruction);
	void MaskBitSetting(uint32_t instruction);
	void gp1write(uint32_t value);
	void Reset(uint32_t instruction);
	void ResetCommandBuffer(uint32_t instruction);
	void ResetGPUIRQ(uint32_t instruction);
	void DisplayDisable(uint32_t instruction);
	void DMADirection(uint32_t instruction);
	void DisplayAreaStart(uint32_t instruction);
	void HorizontalDisplayRange(uint32_t instruction);
	void VerticalDisplayRange(uint32_t instruction);
	void DisplayMode(uint32_t instruction);

	Renderer& m_renderer;
	GP0Mode m_gp0Mode = GP0Mode::Command;
	CommandBuffer m_gp0CommandBuffer;
	uint32_t m_gp0RemainigWords = 0;
	std::function<void()> m_gp0CommandFunc = nullptr;
	Renderer::ImageData m_gp0ImageData;

	union {
		struct {
			uint32_t pageBaseX             : 4; // 0-3
			uint32_t pageBaseY             : 1; // 4
			uint32_t semiTransparency      : 2; // 5-6
			uint32_t pageColorDepth        : 2; // 7-8
			uint32_t dithering             : 1; // 9
			uint32_t drawToDisplay         : 1; // 10
			uint32_t forceSetMask          : 1; // 11
			uint32_t preserveMasked        : 1; // 12
			uint32_t field                 : 1; // 13
			uint32_t reverse               : 1; // 14 NOTE: watch out for this one!
			uint32_t textureDisable        : 1; // 15
			uint32_t horizontalResolution2 : 1; // 16
			uint32_t horizontalResolution1 : 2; // 17-18
			uint32_t verticalResolution    : 1; // 19
			uint32_t videoMode             : 1; // 20
			uint32_t displayColorDepth     : 1; // 21
			uint32_t interlace             : 1; // 22
			uint32_t displayDisable        : 1; // 23
			uint32_t interrupt             : 1; // 24
			uint32_t DMAorDaraRequest      : 1; // 25
			uint32_t receiveCMDReady       : 1; // 26
			uint32_t sendVRAMReady         : 1; // 27
			uint32_t receiveDMAReady       : 1; // 28
			uint32_t DMADirection          : 2; // 29-30
			uint32_t drawOddInInterlaced   : 1; // 31
		} fields;
		uint32_t word = 0x800000; // all 0s except displayDisable
	} m_status;
	bool m_rectangleTextureXFlip = false;
	bool m_rectangleTextureYFlip = false;
	uint16_t m_vramDisplayStartX = 0;
	uint16_t m_vramDisplayStartY = 0;
	uint16_t m_displayHorizontalStart = 0;
	uint16_t m_displayHorizontalEnd = 0;
	uint16_t m_displayLineStart = 0;
	uint16_t m_displayLineEnd = 0;
	uint8_t m_textureWindowMaskX = 0;
	uint8_t m_textureWindowMaskY = 0;
	uint8_t m_textureWindowOffsetX = 0;
	uint8_t m_textureWindowOffsetY = 0;
	uint16_t m_drawingAreaLeft = 0;
	uint16_t m_drawingAreaTop = 0;
	uint16_t m_drawingAreaRight = 0;
	uint16_t m_drawingAreaBottom = 0;
	int16_t m_drawingOffsetX = 0;
	int16_t m_drawingOffsetY = 0;
};
