#pragma once
#include <ctime>

#ifdef _DEBUG
int printf( const char * _Format, ...);
#endif

class stdTimer 
{
public:
#ifdef _DEBUG
	stdTimer()
	{
		printf( "Timer is stdTimer\n");
	}
#endif
	// Reset the timer
	inline void ReStart()
	{
		start = std::clock();
	}
	// Save the result
	inline double GetTime()
	{
		double elapsed = (double)std::clock() - start;
		elapsed /= CLOCKS_PER_SEC;
		return elapsed;
	}
private:
	std::clock_t start;
};

