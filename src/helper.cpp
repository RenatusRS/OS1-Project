#include "helper.h"

volatile int locks = 0;
volatile bool queuedContext = false;
volatile bool call = false;

int syncPrintf(const char *format, ...) {
	int res;
	va_list args;
	lock;
	va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
	unlock;
	return res;
}
