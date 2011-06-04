#pragma once
#ifdef WIN32
#include <windows.h>

#ifdef _DEBUG
int printf( const char * _Format, ...);
#endif

class winTimer
{
public:
	LARGE_INTEGER m_PerformanceCount;
	LARGE_INTEGER m_counter_frequency;
public:
	winTimer()
	{
#ifdef _DEBUG
		printf( "Timer is winTimer\n");
#endif
		QueryPerformanceFrequency(&m_counter_frequency);
	}
	inline void  ReStart()
	{
		QueryPerformanceCounter(&m_PerformanceCount);
	}
	inline double GetTime()
	{
		LARGE_INTEGER pcount;
		QueryPerformanceCounter(&pcount);
		LARGE_INTEGER count_diff;
		count_diff.QuadPart = pcount.QuadPart - m_PerformanceCount.QuadPart;
		double time_diff = (double) count_diff.QuadPart / (double) m_counter_frequency.QuadPart;
		return time_diff;
	}
};

#endif
