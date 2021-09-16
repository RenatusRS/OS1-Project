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
extern volatile bool contextReady;
extern volatile bool call;

// Functions

int syncPrintf(const char *format, ...);

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

#include "uthread.h"

#endif
