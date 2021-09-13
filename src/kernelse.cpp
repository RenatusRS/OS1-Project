#include "helper.h"

volatile Vector<KernelSem *> KernelSem::semaphores;

KernelSem::KernelSem(int init) : val(init) {
	//syncPrintf("KernelSem konstruktor start");

	lock;

	semaphores.pushf(this);

	unlock;
}

KernelSem::~KernelSem() {
	//syncPrintf("KernelSem destruktor start");

	lock;

	for (--semaphores; semaphores.get() != this; semaphores++);
	semaphores.remove();

	unlock;
}

int KernelSem::wait(Time maxTimeToWait) {
	//syncPrintf("KernelSem wait start\n");

	lock;

	PCB::running->semaphorSignaled = false;

	if (--val < 0) {
		// syncPrintf("\n\nThread blocked %d\n\n",PCB::running->id);
		PCB::running->semaphorTime = PCB::running->semaphorLeft = maxTimeToWait;
		PCB::running->state = SUSPENDED;
		block.pushb((PCB *) PCB::running);

		dispatch();
	}

	unlock;

	//syncPrintf("KernelSem wait end %d %d %d\n", maxTimeToWait, value, val);

	return PCB::running->semaphorSignaled;
}

void KernelSem::signal() {
	//syncPrintf("KernemSem signal %d\n",val);

	lock;

	if (++val <= 0) {
		PCB *temp = block.popb();
		// syncPrintf("\n\nThread unblocked %d\n\n",temp->id);
		temp->state = READY;
		temp->semaphorSignaled = true;
		Scheduler::put(temp);
	}

	unlock;
}

void KernelSem::decrease() {
	//syncPrintf("KernemSem decrease\n");

	KernelSem *semaphore;
	PCB *pcb;

	for (--semaphores; (semaphore = semaphores.get()) != nullptr; semaphores++) {
		for (--semaphore->block; (pcb = semaphore->block.get()) != nullptr; semaphore->block++) {
			if (pcb->semaphorTime != 0 && --pcb->semaphorLeft == 0) {
				// syncPrintf("\n\nThread unblocked %d\n\n",pcb->id);
				pcb->state = READY;
				semaphore->block.remove();
				Scheduler::put(pcb);

				semaphore->val++;
			}
		}
	}
}
