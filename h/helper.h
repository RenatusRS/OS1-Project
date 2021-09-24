#ifndef _helper_h_
#define _helper_h_

// Libraries

#include <dos.h>
#include <iostream.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

// Defines

#define bool int
#define nullptr 0
#define true 1
#define false 0

#define intd asm { pushf; cli; }
#define inte asm popf

typedef void interrupt(*pInterrupt)(...);

#define lock locks++
#define unlock if (--locks == 0 && contextReady) { dispatch(); }

// Externs

extern volatile int locks;
extern volatile bool call;
extern volatile bool contextReady;

// Headers

#include "SCHEDULE.h"
#include "vector.h"

#include "thread.h"
#include "pcb.h"

#include "timer.h"

#include "semaphor.h"
#include "kernelse.h"

#include "event.h"
#include "kernelev.h"
#include "ivtentry.h"

// UserThread

int userMain(int argc, char **argv);

class UserThread : public Thread {
	int argc;
	char **argv;

public:
	int ret;

	UserThread(int argc, char **argv) : Thread(), argc(argc), argv(argv), ret(-1) {}

	~UserThread() { waitToComplete(); }

	Thread *clone() const { return new UserThread(argc, argv); }

	void run() { ret = userMain(argc, argv); }

};

#endif
