#pragma once
#include "Pos.h"
#include "SGAmath.h"

/// An axis aligned bounding box.
struct AABB
{
	/// Verify that the bounds are sorted.
	bool IsValid() const;

	/// Get the center of the AABB.
	Pos GetCenter() const
	{
		return 0.5f * (lowerBound + upperBound);
	}

	/// Get the extents of the AABB (half-widths).
	Pos GetExtents() const
	{
		return 0.5f * (upperBound - lowerBound);
	}

	/// Combine two AABBs into this one.
	void Combine(const AABB& aabb1, const AABB& aabb2)
	{
		lowerBound = aabb1.lowerBound.Max(aabb2.lowerBound);
		upperBound = aabb1.upperBound.Max(aabb2.upperBound);
	}

	/// Does this aabb contain the provided AABB.
	bool Contains(const AABB& aabb) const
	{
		bool result = true;
		result = result && lowerBound.x <= aabb.lowerBound.x;
		result = result && lowerBound.y <= aabb.lowerBound.y;
		result = result && aabb.upperBound.x <= upperBound.x;
		result = result && aabb.upperBound.y <= upperBound.y;
		return result;
	}

	Pos lowerBound;	///< the lower vertex
	Pos upperBound;	///< the upper vertex
};