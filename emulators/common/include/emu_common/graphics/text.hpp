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

		explicit SDLText(const SDLFont& font, const char* text = "") : m_font{ font } { setText(text); };
		~SDLText();

		void setText(const char* text);
		void setCharacterSize(unsigned int size);
		void setColor(Color color);
		void setAlign(Align align);
		Color getColor() const { return m_color; }
		Align getAlign() const { return m_align; }
		Vec2u getSize();

		void render(SDL_Renderer* renderer, Transform transform = {}) override;
	private:
		const SDLFont& m_font;
		unsigned int m_characterSize{ 14 };
		unsigned int m_lineHeight;
		Color m_color;
		Align m_align{ Align::Left };
		Vec2u m_size;
		std::vector<std::string> m_text;
		SDLTexture m_texture;
		bool m_isTextureDirty = true;
		bool m_isSizeDirty = true;
	};

} // namespace EmuCommon
