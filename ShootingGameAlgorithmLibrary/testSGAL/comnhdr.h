#define _CRT_SECURE_NO_DEPRECATE
#include <new>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef _MANAGED
#pragma managed(push, off)
#endif

#if defined(_MSC_VER) && (_MSC_VER <= 1200)
	#include <time.h>
	#define currentTime() 		clock()
	#define elapsedTime(x) 		((1000*(x))/CLOCKS_PER_SEC)
#else
    #include "BenchTicks.hpp"
    #include "BenchMemory.hpp"

    #define currentTime() 		stats_rtclock()
    #define elapsedTime(x) 		(x)

	/* Get the current time in milliseconds */
	static unsigned
	stats_rtclock( void )
	{
		struct timeval t;
		#if defined(_WIN32)
		if (gettimeofday( &t, 0) == -1)
			return 0;
		#else
		struct timezone tz;

		if (gettimeofday( &t, &tz ) == -1)
			return 0;
		#endif
		return (t.tv_sec * 1000 + t.tv_usec / 1000);
	}

#endif

#ifdef _MANAGED
#pragma managed(push, on)
#endif

