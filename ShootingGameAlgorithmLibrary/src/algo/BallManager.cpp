#include "BallManager.h"

void BallManager::SetNumThreads( int i )
{
	if (i<=0) i=1;
	if (i>SGA_MAX_THREADS) i=SGA_MAX_THREADS;
}

void BallManager::AddTrajectory( Trajectory* t )
{
	mTrajectoryRawPtrs.push_back(t);
}
