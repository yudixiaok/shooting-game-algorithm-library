/*! @brief 快速又準確的Timer
使用x86的rdtsc實作
*/
#pragma once

#ifdef _WIN32
#include <windows.h>
#include <PowrProf.h>
typedef struct _PROCESSOR_POWER_INFORMATION {
	ULONG Number;
	ULONG MaxMhz;
	ULONG CurrentMhz;
	ULONG MhzLimit;
	ULONG MaxIdleState;
	ULONG CurrentIdleState;
} PROCESSOR_POWER_INFORMATION, *PPROCESSOR_POWER_INFORMATION;
#pragma comment(lib, "PowrProf.lib")
#else
#include <unistd.h>
#include <sys/time.h>
#endif

#ifdef _DEBUG
int printf( const char * _Format, ...);
#endif

namespace
{
	double second_per_tick = 0;
}
#ifdef _WIN32
class rdtscTimer
{
public:
	unsigned __int64 m_start_tick;
	unsigned __int64 m_end_tick;
private:
#ifdef _M_IX86
	inline unsigned __int64 GetCPUTickCount()
	{
		//__asm cpuid;
		__asm rdtsc;
	}
#elif _M_X64
#define GetCPUTickCount() __rdtsc()
#endif
public:
	rdtscTimer()
	{
#ifdef _DEBUG
		printf( "Timer is rdtscTimer\n");
#endif
		if (0 == second_per_tick)
		{
			PROCESSOR_POWER_INFORMATION info;
			CallNtPowerInformation(POWER_INFORMATION_LEVEL::ProcessorInformation, NULL, 0, &info, sizeof(info));
			second_per_tick = info.CurrentMhz;
		}
	}
	/// 更新起始點為現在
	inline void ReStart()
	{
		m_start_tick = GetCPUTickCount();
	}
	/// 得到以秒為單位的時間
	inline double GetTime()
	{
		m_end_tick = GetCPUTickCount();
		return double(m_end_tick-m_start_tick)/second_per_tick;
	}
};
#else  //linux

typedef unsigned long long ticks;

class rdtscTimer
{
;;;	ticks m_start_tick;
	ticks m_end_tick;
	double m_dCount;
	inline ticks rdtsc(void) {
		unsigned a, d;
		asm("cpuid");
		asm volatile("rdtsc" : "=a" (a), "=d" (d));
		return (((ticks)a) | (((ticks)d) << 32));
	}
public:
	rdtscTimer()
	{
		if (second_per_tick)
		{
			timeval tmptime;
			gettimeofday(&tmptime, NULL);
			m_dCount = tmptime.tv_sec + tmptime.tv_usec*.000001;
			double time_diff = 0.0;
			ticks start = rdtsc(), end;
			for (;time_diff<0.01;) // 0.01 second brief a short time
			{
				gettimeofday(&tmptime, NULL);
				time_diff = tmptime.tv_sec + tmptime.tv_usec*.000001 - m_dCount;
			}
			end = rdtsc();
			second_per_tick = double(end - start)/time_diff;
		}
	}
	/// 更新起始點為現在
	inline void ReStart()
	{
		m_start_tick = rdtsc();
	}
	/// 得到以秒為單位的時間
	inline double GetTime()
	{
		m_end_tick = rdtsc();
		return double(m_end_tick-m_start_tick)/second_per_tick;
	}
};

#endif
