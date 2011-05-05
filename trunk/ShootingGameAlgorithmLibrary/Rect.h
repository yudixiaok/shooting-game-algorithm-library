#pragma once
#include "Pos.h"
#include <cmath>
struct Rect
{
	float	left, right, top, bottom;
	Rect(float _left, float _right, float _top, float _bottom) :
	left(_left),
	right(_right),
	top(_top),
	bottom(_bottom)
	{ }

	Rect()					{ }
	/// move x position
	Rect	Movex(const float &num)	{ left += num; right += num; return *this; }
	/// move y position
	Rect	Movey(const float &num)	{ top += num; bottom += num; return *this; }
	/// get rect Width
	float		Width()			{ return fabs(right - left); }
	/// get rect Height
	float		Height()		{ return fabs(bottom - top); }
	/// add anthor rect position
	const Rect operator+(const Rect &rect) const
	{
		Rect	tmp;
		tmp.left = left + rect.left;
		tmp.right = right + rect.right;
		tmp.top = top + rect.top;
		tmp.bottom = bottom + rect.bottom;
		return tmp;
	}

	const Rect operator+(const Pos &pos) const
	{
		Rect	tmp;
		tmp.left = left + pos.x;
		tmp.right = right + pos.x;
		tmp.top = top + pos.y;
		tmp.bottom = bottom + pos.y;
		return tmp;
	}

	Rect &operator+=(const Rect &rect)
	{
		left += rect.left;
		right += rect.right;
		top += rect.top;
		bottom += rect.bottom;
		return *this;
	}

	Rect &operator+=(const Pos &pos)
	{
		left += pos.x;
		right += pos.x;
		top += pos.y;
		bottom += pos.y;
		return *this;
	}

	const Rect operator-(const Rect &rect) const
	{
		Rect	tmp;
		tmp.left = left - rect.left;
		tmp.right = right - rect.right;
		tmp.top = top - rect.top;
		tmp.bottom = bottom - rect.bottom;
		return tmp;
	}

	const Rect operator-(const Pos &pos) const
	{
		Rect	tmp;
		tmp.left = left - pos.x;
		tmp.right = right - pos.x;
		tmp.top = top - pos.y;
		tmp.bottom = bottom - pos.y;
		return tmp;
	}

	Rect &operator-=(const Rect &rect)
	{
		left -= rect.left;
		right -= rect.right;
		top -= rect.top;
		bottom -= rect.bottom;
		return *this;
	}

	Rect &operator-=(const Pos &pos)
	{
		left -= pos.x;
		right -= pos.x;
		top -= pos.y;
		bottom -= pos.y;
		return *this;
	}

	const Rect operator*(const float num) const
	{
		Rect	tmp;
		tmp.left = left * num;
		tmp.right = right * num;
		tmp.top = top * num;
		tmp.bottom = bottom * num;
		return tmp;
	}

	Rect &operator	*=(const float num)
	{ left *= num; right *= num; top *= num; bottom *= num; return *this; }
	const Rect operator/(const float num) const
	{
		Rect	tmp;
		tmp.left = left / num;
		tmp.right = right / num;
		tmp.top = top / num;
		tmp.bottom = bottom / num;
		return tmp;
	}

	Rect &operator	/=(const float num)		{ left /= num; right /= num; top /= num; bottom /= num; return *this; }
	Rect &operator=(const Rect &rect)
	{
		left = rect.left;
		right = rect.right;
		top = rect.top;
		bottom = rect.bottom;
		return *this;
	}

	bool operator==(const Rect &rect) const
	{
		return (left == rect.left && right == rect.right && top == rect.top && bottom == rect.bottom);
	}

	bool operator	!=(const Rect &rect) const	{ return !operator ==(rect); }
	bool isCollision(const Pos &pos) const
	{
		if ((pos.x > left && pos.x < right) && (pos.y > top && pos.y < bottom))
			return true;
		return false;
	}

	bool isCollision(const Rect &rect) const
	{
		Pos p1(left, top), p2(right, bottom);
		if (rect.isCollision(p1) && rect.isCollision(p2))
			return true;
		return false;
	}
};
