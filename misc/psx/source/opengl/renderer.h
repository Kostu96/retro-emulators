#pragma once
#include "buffers.h"
#include "shader.h"

#include <vector>

struct GLFWwindow;

class Renderer
{
public:
	struct ImageData
	{
		uint16_t destinationX;
		uint16_t destinationY;
		uint16_t width;
		uint16_t height;
		std::vector<uint32_t> image;
	};

	struct GouraudVertex
	{
		int16_t x;
		int16_t y;

		union {
			struct {
				uint8_t r;
				uint8_t g;
				uint8_t b;
				uint8_t a;
			};
			uint32_t word;
		} color;
	};

	struct TextureVertex
	{
		int16_t x;
		int16_t y;
		float texCoordX;
		float texCoordY;

		union {
			struct {
				uint8_t r;
				uint8_t g;
				uint8_t b;
				uint8_t a;
			};
			uint32_t word;
		} color;
	};

	Renderer(GLFWwindow* window);

	void swap() const;
	void pushImage(const ImageData& imageData) const;
	void pushTriangle(const void* vertices, size_t size) const;
	void pushQuad(const void* vertices, size_t size) const;
private:
	GLFWwindow* m_window;
	VertexBuffer m_gouraudVBO;
	VertexBuffer m_textureVBO;
	uint32_t m_gouraudVAO;
	uint32_t m_textureVAO;
	Shader m_gouraudShader;
	Shader m_textureShader;
};
