#pragma once

#include "Vec2.h"
#include <math.h>

template<typename T>
class Rect_
{
public:
	Rect_() = default;
	constexpr Rect_(T left_in, T right_in, T top_in, T bottom_in)
		:
		left(left_in),
		right(right_in),
		top(top_in),
		bottom(bottom_in)
	{
	}
	Rect_(const Vec2_<T>& topLeft, const Vec2_<T>& bottomRight)
		:
		Rect_(topLeft.x, bottomRight.x, topLeft.y, bottomRight.y)
	{
	}
	Rect_(const Vec2_<T>& topLeft, T width, T height)
		:
		Rect_(topLeft, topLeft + Vec2_<T>(width, height))
	{
	}
	template<typename S>
	constexpr explicit Rect_(const Rect_<S>& src)
		:
		left((T)src.left),
		right((T)src.right),
		top((T)src.top),
		bottom((T)src.bottom)
	{
	}
	Rect_ GetRounded() const {
		return Rect_{ std::round(left), std::round(right), std::round(top), std::round(bottom) };
	}
	Vec2_<T> GetSizes() const
	{
		return Vec2_<T>(right - left, bottom - top);
	}
	bool IsOverlappingWith(const Rect_& other) const
	{
		return right > other.left && left < other.right
			&& bottom > other.top && top < other.bottom;
	}
	bool IsContainedBy(const Rect_& other) const
	{
		return left >= other.left && right <= other.right &&
			top >= other.top && bottom <= other.bottom;
	}
	bool IsContains(const Vec2_<T>& point) const
	{
		return point.x >= left && point.x < right && point.y >= top && point.y < bottom;
	}
	static constexpr Rect_ FromCenter(const Vec2_<T>& center, T halfWidth, T halfHeight)
	{
		const Vec2_<T> half(halfWidth, halfHeight);
		return Rect_(center - half, center + half);
	}
	Rect_ GetExpanded(T offset) const
	{
		return Rect_(left - offset, right + offset, top - offset, bottom + offset);
	}
	Rect_ GetExpanded(T topOffset, T rightOffset, T bottomOffset, T leftOffset) const
	{
		return Rect_(left - topOffset, right + rightOffset, top - topOffset, bottom + bottomOffset);
	}
	Rect_ GetExpandedWidth(T offset) const
	{
		return Rect_(left - offset, right + offset, top, bottom);
	}
	Vec2_<T> GetCenter() const
	{
		return Vec2_<T>((left + right) / (T)2, (top + bottom) / (T)2);
	}
	Vec2_<T> GetPos() const
	{
		return Vec2_<T>(left, top);
	}
	T GetWidth() const
	{
		return right - left;
	}
	T GetHeight() const
	{
		return bottom - top;
	}
public:
	T left;
	T right;
	T top;
	T bottom;
};

typedef Rect_<int> RectI;
typedef Rect_<float> RectF;