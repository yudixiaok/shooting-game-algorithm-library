#pragma once
#include "ball.h"
#include "behavior.h"
#include <OgreVector3.h>

class Straight : public Behavior
{
public:
	float	mVelocity;
public:
	Straight():mVelocity(1.0f)
	{}
	void SetVector(float v)
	{
		mVelocity = v;
	}
	virtual int UpdateBall(Ball* b, float elapsedtime)
	{
		b->mPosition += b->mDirection * mVelocity * (b->mTimeRate * elapsedtime);
		return Ball::FLY;
	}
	virtual ~Straight()
	{}
};

