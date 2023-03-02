#pragma once

namespace EmuCommon
{
	template <typename T>
	struct Rect
	{
		Rect() = default;
		Rect(T x_, T y_, T width_, T height_) :
			x{ x_ }, y{ y_ },
			width{ width_ }, height{ height_ } {}

		T x{};
		T y{};
		T width{};
		T height{};
	};

	using IRect = Rect<int>;
	using URect = Rect<unsigned int>;
	using FRect = Rect<float>;
}
