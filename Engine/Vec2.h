#pragma once

#include <cmath>

template<typename T>
class Vec2_
{
public:
	Vec2_() = default;
	constexpr Vec2_( T x_in,T y_in )
		:
		x( x_in ),
		y( y_in )
	{}
	template<typename S>
	constexpr explicit Vec2_( const Vec2_<S>& src )
		:
		x( (T)src.x ),
		y( (T)src.y )
	{}
	Vec2_ GetRounded() const
	{
		return Vec2_(std::round(x), std::round(y));
	}
	Vec2_ operator+( const Vec2_& rhs ) const
	{
		return Vec2_( x + rhs.x,y + rhs.y );
	}
	Vec2_& operator+=( const Vec2_& rhs )
	{
		return *this = *this + rhs;
	}
	Vec2_ operator*( T rhs ) const
	{
		return Vec2_( x * rhs,y * rhs );
	}
	Vec2_& operator*=( T rhs )
	{
		return *this = *this * rhs;
	}
	Vec2_ operator/(T rhs) const
	{
		return Vec2_(x / rhs, y / rhs);
	}
	Vec2_& operator/=(T rhs)
	{
		return *this = *this / rhs;
	}
	Vec2_ operator-( const Vec2_& rhs ) const
	{
		return Vec2_( x - rhs.x,y - rhs.y );
	}
	Vec2_& operator-=( const Vec2_& rhs )
	{
		return *this = *this - rhs;
	}
	Vec2_ operator-() const {
		return Vec2_(-x, -y);
	}
	bool operator==(const Vec2_& rhs) const
	{
		return this->x == rhs.x && this->y == rhs.y;
	}
	T GetLength() const
	{
		return (T)std::sqrt( GetLengthSq() );
	}
	T GetLengthSq() const
	{
		return x * x + y * y;
	}
	Vec2_& Normalize()
	{
		return *this = GetNormalized();
	}
	Vec2_ GetNormalized() const
	{
		const T len = GetLength();
		if( len != (T)0 )
		{
			return *this * ((T)1 / len);
		}
		return *this;
	}
public:
	T x;
	T y;
};

typedef Vec2_<float> Vec2;
typedef Vec2_<int> Vei2;