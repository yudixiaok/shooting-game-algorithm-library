#include "BehaviorAnimator.h"
#include <algorithm>

void BehaviorAnimator::AddBehavior( Behavior_Sptr b, float time )
{
	mBehaviorFrames.push_back(BehaviorFrame(b, time));
}

void BehaviorAnimator::AddBehaviorFrame( BehaviorFrame b )
{
	mBehaviorFrames.push_back(b);
}

void BehaviorAnimator::Sort()
{
	sort(mBehaviorFrames.begin(), mBehaviorFrames.end());
}
