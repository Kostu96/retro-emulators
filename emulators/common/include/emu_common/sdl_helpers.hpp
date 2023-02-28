#pragma once
#include <string>

struct SDL_Renderer;
struct SDL_Texture;
typedef struct _TTF_Font TTF_Font;

namespace EmuCommon {

	struct Vec2i { int x = 0, y = 0; };
	struct Color { uint8_t r = 0, g = 0, b = 0, a = 255; };

	class SDLTexture
	{
	public:
		SDLTexture() = default;
		~SDLTexture();

		bool loadFromFile(const char* filename);

		Vec2i getSize() const { return m_size; }

		operator SDL_Texture*() { return m_handle; }
	private:
		SDL_Texture* m_handle = nullptr;
		Vec2i m_size;
	};

	class SDLFont
	{
	public:
		SDLFont() = default;
		~SDLFont();

		bool loadFromFile(const char* filename);
	private:
		static constexpr int DEFAULT_SIZE = 16;

		void setSize(int size);
		operator TTF_Font*() { return m_handle; }

		TTF_Font* m_handle = nullptr;

		friend class SDLText;
	};

	class SDLText
	{
	public:
		SDLText(SDLFont& font) : m_font{ font } {};
		~SDLText();

		SDL_Texture* getTexture() { return m_texture; }

		void setText(const char* text);
		void setTextSize(unsigned int size);
		void setColor(Color color) { m_color = color; }
		void setPosition(Vec2i position) { m_position = position; }
		Vec2i getPosition() const { return m_position; }
		Vec2i getSize() const { return m_size; }

		void render(SDL_Renderer* renderer);
	private:
		SDLFont& m_font;
		SDL_Texture* m_texture = nullptr;
		std::string m_text{ "" };
		unsigned int m_textSize{ SDLFont::DEFAULT_SIZE };
		Color m_color{ 255, 255, 255, 255 };
		Vec2i m_position;
		Vec2i m_size;
	};

} // namespace EmuCommon
