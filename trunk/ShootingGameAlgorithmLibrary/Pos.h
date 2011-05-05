#pragma once
#include <cmath>
#include <cassert>
#include <algorithm>

struct Pos
{
	float	x, y;
	Pos()
	{ }
	Pos (float _x, float _y)
	:
	x(_x),
	y(_y)
	{ }
	Pos (const float *v)
	:
	x(v[0]),
	y(v[1])
	{ }

	/************************************************************************/
	inline Pos operator + ( const Pos& pos ) const
	{
		return Pos(
			x + pos.x,
			y + pos.y);
	}

	inline Pos operator - ( const Pos& pos ) const
	{
		return Pos(
			x - pos.x,
			y - pos.y);
	}

	inline Pos operator * ( const float fScalar ) const
	{
		return Pos(
			x * fScalar,
			y * fScalar);
	}

	inline Pos operator * ( const Pos& rhs) const
	{
		return Pos(
			x * rhs.x,
			y * rhs.y);
	}

	inline Pos operator / ( const float fScalar ) const
	{
		assert( fScalar != 0.0 );

		float fInv = 1.0f / fScalar;

		return Pos(
			x * fInv,
			y * fInv);
	}

	inline Pos operator / ( const Pos& rhs) const
	{
		return Pos(
			x / rhs.x,
			y / rhs.y);
	}

	inline const Pos& operator + () const
	{
		return *this;
	}

	inline Pos operator - () const
	{
		return Pos(-x, -y);
	}

	// overloaded operators to help Pos
	inline friend Pos operator * ( const float fScalar, const Pos& pos )
	{
		return Pos(
			fScalar * pos.x,
			fScalar * pos.y);
	}

	inline friend Pos operator / ( const float fScalar, const Pos& pos )
	{
		return Pos(
			fScalar / pos.x,
			fScalar / pos.y);
	}

	inline friend Pos operator + (const Pos& lhs, const float rhs)
	{
		return Pos(
			lhs.x + rhs,
			lhs.y + rhs);
	}

	inline friend Pos operator + (const float lhs, const Pos& rhs)
	{
		return Pos(
			lhs + rhs.x,
			lhs + rhs.y);
	}

	inline friend Pos operator - (const Pos& lhs, const float rhs)
	{
		return Pos(
			lhs.x - rhs,
			lhs.y - rhs);
	}

	inline friend Pos operator - (const float lhs, const Pos& rhs)
	{
		return Pos(
			lhs - rhs.x,
			lhs - rhs.y);
	}
	// arithmetic updates
	inline Pos& operator += ( const Pos& pos )
	{
		x += pos.x;
		y += pos.y;

		return *this;
	}

	inline Pos& operator += ( const float fScaler )
	{
		x += fScaler;
		y += fScaler;

		return *this;
	}

	inline Pos& operator -= ( const Pos& pos )
	{
		x -= pos.x;
		y -= pos.y;

		return *this;
	}

	inline Pos& operator -= ( const float fScaler )
	{
		x -= fScaler;
		y -= fScaler;

		return *this;
	}

	inline Pos& operator *= ( const float fScalar )
	{
		x *= fScalar;
		y *= fScalar;

		return *this;
	}

	inline Pos& operator *= ( const Pos& pos )
	{
		x *= pos.x;
		y *= pos.y;

		return *this;
	}

	inline Pos& operator /= ( const float fScalar )
	{
		assert( fScalar != 0.0 );

		float fInv = 1.0f / fScalar;

		x *= fInv;
		y *= fInv;

		return *this;
	}

	inline Pos& operator /= ( const Pos& pos )
	{
		x /= pos.x;
		y /= pos.y;

		return *this;
	}
	/************************************************************************/
	Pos &operator=(const Pos &pos)
	{
		x = pos.x;
		y = pos.y;
		return *this;
	};
	bool operator==(const Pos &pos) const
	{
		return (pos.x == x) && (pos.y == y);
	};
	bool operator!=(const Pos &pos) const
	{
		return (pos.x != x) || (pos.y != y);
	};

	/************************************************************************/
	Pos Max(const Pos& pos) const
	{
		return Pos(max(x, pos.x), max(y, pos.y));
	}
	Pos Min(const Pos& pos) const
	{
		return Pos(min(x, pos.x), min(y, pos.y));
	}
	float length() const
	{
		return sqrt((float) (x * x + y * y));
	};
	float squaredLength() const
	{
		return x * x + y * y;
	};
	float distance(const Pos &pos) const
	{
		return (*this - pos).length();
	}
	Pos &inverse()
	{
		x = -x;
		y = -y;
		return *this;
	}

	float normalise()
	{
		float	fLength = length();
		if (fLength > 1e-08)
		{
			float	fInvLength = 1.0f / fLength;
			x *= fInvLength;
			y *= fInvLength;
		}

		return fLength;
	}

	Pos midPoint(const Pos &vec) const
	{
		return Pos((x + vec.x) * 0.5f, (y + vec.y) * 0.5f);
	}

	float dotProduct(const Pos &vec) const
	{
		return x * vec.x + y * vec.y;
	}

	float crossProduct(const Pos &pos) const
	{
		return x * pos.y - y * pos.x;
	}

	Pos reflect(const Pos &normal) const
	{
		return Pos(*this - (2 * this->dotProduct(normal) * normal));
	}

	/** Function for writing to a stream.
        */
	inline friend std::ostream & operator <<(std::ostream & o, const Pos & v)
	{
		o << "Pos(" << v.x << ", " << v.y << ")";
		return o;
	}
	inline void swap(Pos &other)
	{
		std::swap(x, other.x);
		std::swap(y, other.y);
	}
};
