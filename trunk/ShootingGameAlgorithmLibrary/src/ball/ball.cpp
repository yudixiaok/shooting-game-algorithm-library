#include "ball.h"

int Ball::Update( float elapsedtime )
{
	if (mpBehavior.get())
		return mpBehavior->UpdateBall(this, elapsedtime);
	return STOP;
}

bool Ball::HasBehavior()
{
	return mpBehavior.get()!=0;
}

