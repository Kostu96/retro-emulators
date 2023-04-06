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

		TTF_Font* getHandle() const { return m_handle; }
	private:
		static constexpr int DEFAULT_SIZE = 14;

		void setSize(int size) const;

		TTF_Font* m_handle = nullptr;

		friend class SDLText;
	};

} // namespace EmuCommon
