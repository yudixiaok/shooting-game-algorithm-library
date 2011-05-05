#pragma once
#include <vector>
#include "behavior.h"

struct BehaviorFrame
{
	BehaviorFrame(Behavior_Sptr _mBehavior, float _mStartTime)
		:mBehavior(_mBehavior), mStartTime(_mStartTime)
	{}
	Behavior_Sptr mBehavior;
	float	mStartTime;
	bool operator < (const BehaviorFrame& rhs) const
	{
		return mStartTime < rhs.mStartTime;
	}
};
typedef std::vector<BehaviorFrame> BehaviorFrames;
class BehaviorAnimator
{
public:
	BehaviorFrames mBehaviorFrames;
	void AddBehavior(Behavior_Sptr b, float time);
	void AddBehaviorFrame(BehaviorFrame b);
	void Sort();
};

