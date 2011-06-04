
#pragma once
class winTimer;
class rdtscTimer;
class stdTimer;
class unixTimer;
#include "winTimer.h"
#include "rdtscTimer.h"
#include "stdTimer.h"
#include "unixTimer.h"

template<class TIMER_IMP>
class TTimer
{
public:
	enum TimerState
	{
		RUNNING,
		STOPPED,
	};
	TIMER_IMP m_timer;
	int m_status;
	double m_CountTime;
public:
	TTimer():m_status(RUNNING){}
	/// start
	double Start()
	{
		m_status = RUNNING;
		m_timer.ReStart();
		return m_CountTime;
	}
	/// stop
	double Stop()
	{
		m_CountTime += GetTime();
		m_status = STOPPED;
		return m_CountTime;
	}
	/// Restart timer
	void  Restart()
	{
		m_CountTime = 0;
		m_status = RUNNING;
		m_timer.ReStart();
	}
	/// Restart now
	void ReTick()
	{
		m_timer.ReStart();
	}
	/// get time second
	inline double GetTime()
	{
		return m_timer.GetTime();
	}
	inline double GetCountTime()
	{
		if (m_status == STOPPED)
			return m_CountTime;
		return m_CountTime + m_timer.GetTime();
	}
};
namespace timer
{
	typedef TTimer<stdTimer>	std;
	typedef TTimer<rdtscTimer>	rdtsc;
	//typedef TTimer<winTimer>	win;
	//typedef TTimer<unixTimer>	unix;
}

#ifdef WIN32
typedef TTimer<winTimer>	Timer;
#	else
typedef TTimer<unixTimer>	Timer;
#endif

