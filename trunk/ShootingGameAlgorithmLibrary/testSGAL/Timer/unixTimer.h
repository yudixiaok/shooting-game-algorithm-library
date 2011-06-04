#pragma once
#ifndef _WIN32

#ifdef _DEBUG
int printf(stdout, const char * _Format, ...);
#endif

class unixTimer 
{
public:
#ifdef _DEBUG
	unixTimer()
	{
		printf(stdout, "Timer is unixTimer\n");
	}
#endif
	// Reset the timer
	inline void ReStart()
	{
		timeval tmptime;
		gettimeofday(&m_time, NULL);
		m_dCount = tmptime.tv_sec + tmptime.tv_usec*.000001;
	}
	// Save the result
	inline double GetTime()
	{
		timeval tmptime;
		gettimeofday(&tmptime, NULL);
		double time = (tmptime.tv_sec-m_time.tv_sec) + (tmptime.tv_usec-m_time.tv_usec)*.000001;
		return time;
	}
private:
	timeval m_time;
	double m_dCount;
};

#endif
