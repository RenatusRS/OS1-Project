#ifndef _kernesem_h_
#define _kernesem_h_

#include "helper.h"

class KernelSem {
public:
	volatile int val;
	Vector<PCB *> block;

	KernelSem(int init);

	~KernelSem();

	int wait(Time maxTimeToWait);

	void signal();

	volatile static Vector<KernelSem *> semaphores;

	static void decrease();
};


#endif
