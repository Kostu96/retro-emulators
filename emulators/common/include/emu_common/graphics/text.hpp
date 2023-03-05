#pragma once
#include "emu_common/graphics/transformable.hpp"
#include "emu_common/graphics/renderable.hpp"
#include "emu_common/graphics/color.hpp"
#include "emu_common/graphics/texture.hpp"

#include <string>
#include <vector>

namespace EmuCommon {

	class SDLFont;

	class SDLText :
		public Transformable,
		public Renderable
	{
	public:
		enum class Align { Left, Right, Center };

		SDLText() = default;
		explicit SDLText(const SDLFont& font, const char* text = "") : m_font{ &font } { setText(text); };

		void setText(const char* text);
		void setFont(const SDLFont& font);
		void setCharacterSize(unsigned int size);
		void setColor(Color color);
		void setAlign(Align align);
		const std::string& getText() const { return m_text; }
		const SDLFont* getFont() const { return m_font; }
		unsigned int getCharacterSize() const { return m_characterSize; }
		unsigned int getLineHeight() const { return m_lineHeight; }
		Color getColor() const { return m_color; }
		Align getAlign() const { return m_align; }
		Vec2u getSize();

		void render(SDL_Renderer* renderer, Transform transform = {}) override;
	private:
		std::string m_text{ "" };
		const SDLFont* m_font = nullptr;
		unsigned int m_characterSize{ 14 };
		unsigned int m_lineHeight = 0;
		Color m_color;
		Align m_align{ Align::Left };
		Vec2u m_size;

		std::vector<std::string> m_lines;
		SDLTexture m_texture;
		bool m_isTextureDirty = true;
		bool m_isSizeDirty = true;
	};

} // namespace EmuCommon
