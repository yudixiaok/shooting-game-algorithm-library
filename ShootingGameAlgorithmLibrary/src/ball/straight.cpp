#include "straight.h"

void Straight::SetVelocity( float v )
{
	mVelocity = v;
}

int Straight::UpdateBall( Ball* b, float elapsedtime )
{
	b->mPosition += b->mDirection * mVelocity * (b->mTimeRate * elapsedtime);
	return Ball::FLY;
}
