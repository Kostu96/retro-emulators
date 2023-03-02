#pragma once
#include "emu_common/graphics/transformable.hpp"
#include "emu_common/graphics/renderable.hpp"
#include "emu_common/graphics/color.hpp"

#include <string>

struct SDL_Texture;

namespace EmuCommon {

	class SDLFont;
	class SDLTexture;

	class SDLText :
		public Transformable,
		public Renderable
	{
	public:
		explicit SDLText(const SDLFont& font, const char* text = "") : m_font{ font } { setText(text); };
		~SDLText();

		void setText(const char* text);
		void setTextSize(unsigned int size);
		void setColor(Color color) { m_color = color; }
		Vec2u getSize();

		void render(SDL_Renderer* renderer, Vec2f offset) override;
	private:
		const SDLFont& m_font;
		std::string m_text{ "" };
		unsigned int m_textSize{ 14 };
		Color m_color;
		Vec2u m_size;

		SDL_Texture* m_texture = nullptr;
		bool m_isTextureDirty = true;
		bool m_isSizeDirty = true;
		bool m_need2Texture = false;
		SDL_Texture* m_texture2 = nullptr;
		std::string m_str2;
	};

} // namespace EmuCommon
