#pragma once
#include "../common/SGA_config.h"
#ifdef SGA_USE_MUTITHREAD
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
typedef boost::mutex MUTEX;
typedef MUTEX::scoped_lock LOCK;
#endif
#include "../ball/ball.h"
#include "Trajectory.h"
#include "../common/shared_ptr.h"

class BallManager
{
private:
	int		mNumThreads;
	BallVector	mBallVector;
	TrajectoryRawPtrs mTrajectoryRawPtrs;
#ifdef SGA_USE_MUTITHREAD
	boost::thread_group mThreadgroup;
	struct work
	{
		BallVector* bv;
		size_t	begin;
		size_t	end;
	};
	struct work_info
	{
		work_info():work_done(true)
		{}
		MUTEX	m;
		std::vector<work> works;
		float	time;
		bool	work_done;
	};
	SHARE_PTR(work_info)
	work_info* mThreadsWork[SGA_MAX_THREADS];
	bool mOver;
#endif	
public:
#ifdef SGA_USE_MUTITHREAD
	BallManager(int _mNumThreads=1)
		:mNumThreads(_mNumThreads), mOver(false)
	{
		memset(mThreadsWork, 0, sizeof(mThreadsWork));
		if (mNumThreads>1)
		{
			for (int i=0;i<mNumThreads;i++)
			{
				mThreadsWork[i] = new work_info;
				mThreadgroup.create_thread(
					boost::bind(&BallManager::MutiThreadUpdate, boost::ref(*this), i));
			}
		}
	}
	~BallManager()
	{
		mOver = true;
		mThreadgroup.join_all();
		for (int i=0;i<SGA_MAX_THREADS;i++)
		{
			if (mThreadsWork[i])
				delete mThreadsWork[i];
		}
	}
#else
	BallManager(int _mNumThreads=1)
		:mNumThreads(_mNumThreads)
	{
	}
#endif
	void SetNumThreads(int i);
	void AddTrajectory(Trajectory* t);
	void Update(float time)
	{
#ifdef SGA_USE_MUTITHREAD
		TrajectoryRawPtrs::iterator it = mTrajectoryRawPtrs.begin();
		if (mNumThreads>1)
		{
			for (size_t i=0;i<mNumThreads;i++)
			{
				LOCK l(mThreadsWork[i]->m);
				mThreadsWork[i]->works.clear();
			}
			for (;it != mTrajectoryRawPtrs.end();it++)
			{
				BallVector& bv = (*it)->GetBallVector();
				double step = bv.size()/(double)mNumThreads;
				double now_step = 0;
				for (size_t i=0;i<mNumThreads;i++)
				{
					LOCK l(mThreadsWork[i]->m);
					if (mThreadsWork[i]->work_done)
					{
						if ((int)now_step == bv.size())
							break;
						mThreadsWork[i]->work_done = false;
						mThreadsWork[i]->time = time;
						work w;
						w.bv = &bv;
						w.begin = (int)now_step;
						now_step += step;
						if ((size_t)now_step >= bv.size())
							now_step = bv.size();
						w.end = (int)now_step;
						mThreadsWork[i]->works.push_back(w);
					}
				}
			}
		}
		else
		{
			for (;it != mTrajectoryRawPtrs.end();it++)
			{
				BallVector& bv = (*it)->GetBallVector();
				size_t bsize = bv.size();
				for (size_t i=0;i < bsize;i++)
				{
					bv[i].Update(time);
				}
			}
		}
#else
		Trajectory_Sptrs::iterator it = mTrajectoryRawPtrs.begin();
		for (;it != mTrajectoryRawPtrs.end();it++)
		{
			BallVector& bv = it->GetBallVector();
			size_t bsize = bv.size();
			for (size_t i=0;i < bsize;i++)
			{
				bv[i].Update(time);
			}
		}
#endif
	}
#ifdef SGA_USE_MUTITHREAD
	void MutiThreadUpdate(int i)
	{
		for (;!mOver;) // 程式還沒結束
		{
			work_info& info = *mThreadsWork[i];
			if (!info.work_done) // 要工作了
			{
				LOCK l(info.m); //鎖
				size_t numwork = info.works.size();
				for (size_t nw=0;nw<numwork;nw++)
				{
					BallVector& bv = *(info.works[nw].bv);
					size_t bsize = bv.size();
					for (size_t i=info.works[nw].begin;i < info.works[nw].end;i++)
					{
						bv[i].Update(info.time);
					}
				}
				info.work_done = true;
			}
		}
	}
#endif
};
