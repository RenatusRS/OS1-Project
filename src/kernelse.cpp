#include "helper.h"

volatile Vector<KernelSem *> KernelSem::semaphores;

KernelSem::KernelSem(int init) : val(init) {
	lock;

	semaphores.pushf(this);

	unlock;
}

KernelSem::~KernelSem() {
	lock;

	for (--semaphores; semaphores.get() != this; semaphores++);
	semaphores.remove();

	unlock;
}

int KernelSem::wait(Time maxTimeToWait) {
	lock;

	if (--val < 0) {
		PCB::running->semaphorTime = PCB::running->semaphorLeft = maxTimeToWait;
		PCB::running->state = SUSPENDED;
		block.pushb((PCB *) PCB::running);

		dispatch();
	}

	unlock;

	return PCB::running->semaphorSignaled;
}

void KernelSem::signal() {
	lock;

	if (++val <= 0) {
		PCB *temp = block.popb();
		temp->state = READY;
		temp->semaphorSignaled = true;
		Scheduler::put(temp);
	}

	unlock;
}

void KernelSem::decrease() {
	KernelSem *semaphore;
	PCB *pcb;

	for (--semaphores; (semaphore = semaphores.get()) != nullptr; semaphores++) {
		for (--semaphore->block; (pcb = semaphore->block.get()) != nullptr; semaphore->block++) {
			if (pcb->semaphorTime != 0 && --pcb->semaphorLeft == 0) {
				pcb->semaphorSignaled = false;

				pcb->state = READY;
				semaphore->block.remove();
				Scheduler::put(pcb);

				semaphore->val++;
			}
		}
	}
}
