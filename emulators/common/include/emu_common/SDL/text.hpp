#pragma once
#include "emu_common/SDL/transformable.hpp"
#include "emu_common/SDL/renderable.hpp"

#include <string>

struct SDL_Texture;

namespace EmuCommon {

	struct Color { unsigned char r = 0, g = 0, b = 0, a = 255; };

	class SDLFont;
	class SDLTexture;

	class SDLText :
		public Transformable,
		public Renderable
	{
	public:
		explicit SDLText(SDLFont& font, const char* text = "") : m_font{ font } { setText(text); };
		~SDLText();

		void setText(const char* text);
		void setTextSize(unsigned int size);
		void setColor(Color color) { m_color = color; }
		Vec2i getSize();

		void render(SDL_Renderer* renderer, Vec2f offset) override;
	private:
		SDLFont& m_font;
		std::string m_text{ "" };
		unsigned int m_textSize{ 14 };
		Color m_color{ 255, 255, 255, 255 };
		Vec2i m_size;

		SDL_Texture* m_texture = nullptr;
		bool m_isTextureDirty = true;
		bool m_isSizeDirty = true;
		bool m_need2Texture = false;
		SDL_Texture* m_texture2 = nullptr;
		std::string m_str2;
	};

} // namespace EmuCommon
