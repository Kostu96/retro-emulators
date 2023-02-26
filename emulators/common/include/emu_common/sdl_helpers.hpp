#pragma once
#include <string>

struct SDL_Texture;
typedef struct _TTF_Font TTF_Font;

namespace EmuCommon {

	class SDLTexture
	{
	public:
		SDLTexture() = default;
		~SDLTexture();

		bool loadFromFile(const char* filename);

		int getWidth() const { return m_width; }
		int getHeight() const { return m_height; }

		operator SDL_Texture*() { return m_handle; }
	private:
		SDL_Texture* m_handle = nullptr;
		int m_width = 0;
		int m_height = 0;
	};

	class SDLFont
	{
	public:
		SDLFont() = default;
		~SDLFont();

		bool loadFromFile(const char* filename, int size);

		operator TTF_Font*() { return m_handle; }
	private:
		TTF_Font* m_handle;
	};

	class SDLText
	{
	public:
		SDLText(SDLFont& font) : m_font{ font } {};
		~SDLText();

		SDL_Texture* getTexture() { return m_texture; }
		int getWidth() const { return m_textureWidth; }
		int getHeight() const { return m_textureHeight; }
		void setText(const char* str, uint8_t colorR = 0xFF, uint8_t colorG = 0xFF, uint8_t colorB = 0xFF);
	private:
		SDLFont& m_font;
		SDL_Texture* m_texture = nullptr;
		int m_textureWidth = 0;
		int m_textureHeight = 0;
		std::string m_text;
	};

} // namespace EmuCommon
