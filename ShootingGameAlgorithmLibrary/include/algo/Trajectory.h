#pragma once
#include <OgreVector3.h>
#include "../ball/ball.h"
#include "../ball/behavior.h"

class Trajectory
{
public:
	int		mNumTrajectory;
	Ogre::Vector3	mPosition;
	Ogre::Vector3	mDirection;
	Ogre::Vector3	mUp;
	float		mInitializeTime;
	Behavior_Sptr	mBehavior;
public:
	Trajectory(int _mNumTrajectory, Ogre::Vector3 _mPosition, Ogre::Vector3 _mDirection)
		:mNumTrajectory(_mNumTrajectory), mPosition(_mPosition), mDirection(_mDirection),
		mInitializeTime(0), mUp(Ogre::Vector3::UNIT_Z)
	{}
	virtual ~Trajectory()
	{}
	virtual void AddBall(BallList& out)=0;
	virtual void AddBall(BallVector& out)=0;
	virtual BallList	GenerateBallList()=0;
	virtual BallVector	GenerateBallVector()=0;
	virtual void SetBehavior(Behavior_Sptr b) 
	{
		mBehavior = b;
		mNeedUpdate = true;
		CheckModify();
	}
protected:
	BallVector mBall_PreComptue;
	bool		mNeedUpdate;
	virtual void CheckModify()
	{
		if (mNeedUpdate)
			Modifyed();
	}
	virtual void Modifyed()=0;
};
