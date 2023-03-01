#pragma once

namespace EmuCommon {

	template<typename T>
	struct Vec2
	{
		Vec2() = default;
		Vec2(T x_, T y_) :
			x{ x_ }, y{ y_ } {}

		T x{};
		T y{};
	};

	template <typename T>
	Vec2<T> operator-(const Vec2<T>& right) { return { -right.x, -right.y }; }
	
	template <typename T>
	Vec2<T>& operator+=(Vec2<T>& left, const Vec2<T>& right)
	{
		left.x += right.x;
		left.y += right.y;
		return left;
	}
	
	template <typename T>
	Vec2<T>& operator-=(Vec2<T>& left, const Vec2<T>& right)
	{
		left.x -= right.x;
		left.y -= right.y;
		return left;
	}
	
	template <typename T>
	Vec2<T> operator+(Vec2<T> left, const Vec2<T>& right) { return left += right; }
	
	template <typename T>
	Vec2<T> operator-(Vec2<T> left, const Vec2<T>& right) { return left -= right; }
	
	template <typename T>
	Vec2<T> operator*(Vec2<T> left, T right) { return left *= right; }
	
	template <typename T>
	Vec2<T> operator*(T left, const Vec2<T>& right) { return operator*(right, left); }
	
	template <typename T>
	Vec2<T>& operator*=(Vec2<T>& left, T right)
	{
		left.x *= right;
		left.y *= right;
		return left;
	}
	
	template <typename T>
	Vec2<T> operator/(Vec2<T> left, T right) { return left /= right; }
	
	template <typename T>
	Vec2<T>& operator/=(Vec2<T>& left, T right)
	{
		left.x /= right;
		left.y /= right;
		return left;
	}
	
	template <typename T>
	bool operator==(const Vec2<T>& left, const Vec2<T>& right) { return (left.x == right.x) && (left.y == right.y); }
	
	template <typename T>
	bool operator!=(const Vec2<T>& left, const Vec2<T>& right) { return !operator==(left, right); }

	using Vec2i = Vec2<int>;
	using Vec2u = Vec2<unsigned int>;
	using Vec2f = Vec2<float>;

} // namespace EmuCommon
