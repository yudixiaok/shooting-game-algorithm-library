
#if defined(_MSC_VER)
	typedef unsigned __int64 BenchTicks_t;
#else
	typedef unsigned BenchTicks_t;
#endif

	static BenchTicks_t BenchTicksGetFrequency();
	static BenchTicks_t BenchTicksGetCurrent();


#if defined(_WIN32)
#include <windows.h>

#if defined(_M_IX86)
#if _MSC_VER >= 1400
#include <intrin.h>
#pragma intrinsic(__rdtsc)
#pragma intrinsic(_ReadWriteBarrier)

static inline
BenchTicks_t BenchTicksGetCurrent()
{
	return __rdtsc();
}
#elif _MSC_VER >= 1310
static inline
BenchTicks_t BenchTicksGetCurrent()
{
	LARGE_INTEGER retval;
	QueryPerformanceCounter(&retval); 
	return retval.QuadPart;
}
#else 
static inline
BenchTicks_t BenchTicksGetCurrent()
{
	return clock();
}
#endif

static inline int gettimeofday (struct timeval *tv, void* tz) 
{ 
	union { 
		unsigned __int64 ns100; /*time since 1 Jan 1601 in 100ns units */ 
		FILETIME ft; 
	} now; 

	GetSystemTimeAsFileTime (&now.ft); 
	tv->tv_usec = (long) ((now.ns100 / 10LL) % 1000000LL); 
	tv->tv_sec = (long) ((now.ns100 - 116444736000000000LL) / 10000000LL); 
	return (0); 
} 

static inline unsigned currentTime()
{
	struct timeval t;

	if (gettimeofday( &t, 0 ) == -1)
		return 0;
	return (t.tv_sec * 1000 + t.tv_usec / 1000);
}

static inline
BenchTicks_t BenchTicksGetFrequency()
{
	static BenchTicks_t retval = 0;
	if (retval != 0) {
		return retval;
	}
	unsigned nMSec = currentTime();
	BenchTicks_t nTicks = BenchTicksGetCurrent();
	Sleep(500);
	nTicks = BenchTicksGetCurrent() - nTicks;
	nMSec = currentTime() - nMSec;

	retval = nTicks * 1000 / nMSec;
	return retval;
}


#else
static inline
BenchTicks_t BenchTicksGetCurrent()
{
	LARGE_INTEGER retval;
	QueryPerformanceCounter(&retval); 
	return retval.QuadPart;
}

static inline
BenchTicks_t BenchTicksGetFrequency()
{
	LARGE_INTEGER retval;
	QueryPerformanceFrequency(&retval); 
	return retval.QuadPart;
}
#endif

#elif defined(_MACINTOSH)
static inline
BenchTicks_t BenchTicksGetCurrent()
{
	return clock();
}

inline
BenchTicks_t BenchTicksGetFrequency()
{
	return CLOCKS_PER_SEC;
}

#else
#include <sys/time.h>

static inline
BenchTicks_t BenchTicksGetCurrent()
{
	struct timeval t;
	struct timezone tz;

	if (gettimeofday( &t, &tz ) == -1)
		return 0;
	return (t.tv_sec * 1000 + t.tv_usec / 1000);
}

inline
BenchTicks_t BenchTicksGetFrequency()
{
	return 1000;
}

#endif

static
char * BenchTicksToString(BenchTicks_t ticks, bool bShowfps = false, bool bShowTicks = false)
{
	BenchTicks_t tickspersec = BenchTicksGetFrequency();

	static char buf[200]; int len = 0;
	BenchTicks_t sec, msec, usec, nsec;
	BenchTicks_t left = ticks;

	sec = left / tickspersec; left = left % tickspersec;
	msec = (left * 1000) / tickspersec; left = (left * 1000) % tickspersec;
	usec = (left * 1000) / tickspersec; left = (left * 1000) % tickspersec;
	nsec = (left * 1000) / tickspersec; left = (left * 1000) % tickspersec;

#if defined(_MSC_VER)
	if (sec) {
		len += sprintf(buf + len, "%I64u.%03I64u s", sec, msec);
	} else if (msec) {
		len += sprintf(buf + len, "%I64u.%03I64u ms", msec, usec);
	} else {
		len += sprintf(buf + len, "%I64u.%03I64u us", usec, nsec);
	}
#else
	if (sec) {
		sprintf(buf, "%u.%03u sec", sec, msec);
	} else if (msec) {
		sprintf(buf, "%u.%03u msec", msec, usec);
	} else (
		sprintf(buf, "%u.%03u usec", usec, nsec);
	}
#endif
	if (bShowfps) {
		double time = (ticks/(double)tickspersec);
		double manytime = 100-(1/60.0-time)/(1/60.0)*100;
		len += sprintf(buf + len, " (%f fps, %.4f use %.3f%%)", 1.0/time, time, manytime);
	}
	if (bShowTicks) {
#if defined(_MSC_VER)
		len += sprintf(buf + len, " (%I64u ticks)", ticks);
#else
		len += sprintf(buf + len, " (%u ticks)", ticks);
#endif
	}
	return buf;
}

struct CBenchAuto {
	BenchTicks_t * m_pTicks;
	CBenchAuto(BenchTicks_t *p) {
		m_pTicks = p;
		*m_pTicks = BenchTicksGetCurrent();
	}
	~CBenchAuto() {
		*m_pTicks = (BenchTicksGetCurrent() - *m_pTicks) ;
	}
	operator bool () {
		return true;
	}
};
#define BENCHMON(x)			if (CBenchAuto dummy = x)

/* 
Usage:
	BenchTicks_t t1;
	{ CBenchAuto dummy(&t1);
	  ...
	}
	printf("Time elapsed: %s\n", BenchTicksToString(t1));
*/