#pragma once
#include "OgreVector3.h"
#include "behavior.h"
#include <vector>
#include <list>

class Ball
{
public:
	enum BallStatus
	{
		CREATE = 2,
		FLY,
		STOP,
		DESTORY
	};
	Ball():mTimeRate(1)
	{
	}
	Ball(const Ogre::Vector3 pos, const Ogre::Vector3 dir, Behavior_Sptr Behavior = Behavior_Sptr())
		:mTimeRate(1), mPosition(pos), mDirection(dir), mUp(Ogre::Vector3::UNIT_Z), mpBehavior(Behavior)
	{
	}
	int Update(float elapsedtime)
	{
		if (mpBehavior.get())
			return mpBehavior->UpdateBall(this, elapsedtime);
		return STOP;
	}
	bool HasBehavior()
	{
		return mpBehavior.get()!=0;
	}
	Ogre::Vector3	mPosition;
	Ogre::Vector3	mDirection;
	Ogre::Vector3	mUp;
	float		mTimeRate;
	Behavior_Sptr	mpBehavior;
};
typedef std::vector<Ball> BallVector;
typedef std::list<Ball> BallList;
