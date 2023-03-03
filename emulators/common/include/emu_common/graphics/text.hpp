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
		void setColor(Color color) { m_color = color; }
		void setAlign(Align align) { m_align = align; }
		Color getColor() const { return m_color; }
		Align getAlign() const { return m_align; }
		Vec2u getSize();

		void render(SDL_Renderer* renderer, const RenderStates& states = {}) override;
	private:
		void destroyTextures();

		const SDLFont& m_font;
		unsigned int m_characterSize{ 14 };
		float m_lineHeight;
		Color m_color;
		Align m_align{ Align::Left };
		Vec2u m_size;
		std::vector<std::string> m_text;
		std::vector<SDLTexture*> m_textures;
		bool m_isTextureDirty = true;
		bool m_isSizeDirty = true;
	};

} // namespace EmuCommon
