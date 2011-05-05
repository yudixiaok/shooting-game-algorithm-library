#pragma once
#include "limits"
#include "Pos.h"

struct b2RayCastOutput
{
	Pos p1, p2;
};


bool NumIsValid(float x)
{
	if (x != x)
	{
		// NaN.
		return false;
	}

	float infinity = std::numeric_limits<float>::infinity();
	return -infinity < x && x < infinity;
}