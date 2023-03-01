#pragma once

typedef struct _TTF_Font TTF_Font;

namespace EmuCommon {

	class SDLText;

	class SDLFont
	{
	public:
		SDLFont() = default;
		explicit SDLFont(const char* filename) { loadFromFile(filename); }
		~SDLFont();

		bool loadFromFile(const char* filename);
	private:
		static constexpr int DEFAULT_SIZE = 14;

		void setSize(int size);
		operator TTF_Font* () { return m_handle; }

		TTF_Font* m_handle = nullptr;

		friend class SDLText;
	};

} // namespace EmuCommon
