
#if defined(_MSC_VER) && (_MSC_VER > 1200)
#include <windows.h>
#include "Psapi.h"
#pragma comment(lib, "Psapi.lib")

class CBenchMemory {
	PROCESS_MEMORY_COUNTERS m_vPMC0;
    SSIZE_T m_vPeakWorkingSetSize;
    SSIZE_T m_vWorkingSetSize;
public:
	void start();
	void stop();
	static int valueToString(char *buf, SSIZE_T val);
	char * toString();
};

inline
void CBenchMemory::start()
{
	GetProcessMemoryInfo(GetCurrentProcess(), &m_vPMC0, sizeof(PROCESS_MEMORY_COUNTERS));
}

inline
void CBenchMemory::stop()
{
	PROCESS_MEMORY_COUNTERS vPMC;
	GetProcessMemoryInfo(GetCurrentProcess(), &vPMC, sizeof(PROCESS_MEMORY_COUNTERS));
    m_vPeakWorkingSetSize = vPMC.PeakWorkingSetSize - m_vPMC0.PeakWorkingSetSize;
    m_vWorkingSetSize = vPMC.WorkingSetSize - m_vPMC0.WorkingSetSize;
}

inline
int CBenchMemory::valueToString(char *buf, SSIZE_T val)
{
	int retval = 0;
	if (val < 0) {
		retval += sprintf(buf + retval, "-");
		retval += valueToString(buf, -val);
		return retval;
	} else {
		if (val > 1000) {
			retval += valueToString(buf, val / 1000);
			retval += sprintf(buf + retval, ",%03d", val%1000);
		} else {
			retval += sprintf(buf + retval, "%d", val);
		}
		return retval;
	}
}

inline 
char * CBenchMemory::toString()
{
	static char buf[100];
	int len = 0;
	len += sprintf(buf + len, "WorkingSetSize:\n  Before: ");
	len += valueToString(buf + len, m_vPMC0.WorkingSetSize);
	len += sprintf(buf + len, "\n    Peak: ");
	if (m_vPeakWorkingSetSize > 0) {
		len += valueToString(buf + len, m_vPMC0.PeakWorkingSetSize + m_vPeakWorkingSetSize);
	} else if (m_vPeakWorkingSetSize < 0) {
		len += valueToString(buf + len, m_vPeakWorkingSetSize);
	} else {
		len += sprintf(buf + len, "--");
	}
	len += sprintf(buf + len, "\n   After: ");
	if (m_vPMC0.WorkingSetSize + m_vWorkingSetSize == 
		m_vPMC0.PeakWorkingSetSize + m_vPeakWorkingSetSize) {
		len += sprintf(buf + len, "[Same As Peak] ");
	} else {
		len += valueToString(buf + len, m_vPMC0.WorkingSetSize + m_vWorkingSetSize);
	}
	len += sprintf(buf + len, "  Increase: ");
	len += valueToString(buf + len, m_vWorkingSetSize);
	return buf;
}


template <typename T>
class CBenchScope {
	T * m_pCount;
public:
	CBenchScope(T *p) {
		m_pCount = p;
		p->start();
	}
	~CBenchScope() {
		m_pCount->stop();
	}
};
/* 
Usage:
	CBenchMemory m1;
	{ CBenchScope<CBenchMemory> dummy(&m1);
	  ...
	}
	printf("Memory Usage: %s\n", m1.toString());
*/

#endif