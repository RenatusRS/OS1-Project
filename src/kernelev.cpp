#include "helper.h"

KernelEv::KernelEv(IVTNo ivtNo) : ivtNo(ivtNo), pcb((PCB*) PCB::running) {
	IVTEntry::table[ivtNo]->kernelev = this;
}

KernelEv::~KernelEv() {
    intd;
    setvect(ivtNo, IVTEntry::table[ivtNo]->oldRt);
    inte;
}

void KernelEv::wait() {
	if (pcb == PCB::running) {
		pcb->state = SUSPENDED_EVENT;
		dispatch();
	}
}

void KernelEv::signal() {
	lock;
	if (pcb->state == SUSPENDED_EVENT) {
    	pcb->state = READY;
    	Scheduler::put(pcb);
    }
	unlock;

	dispatch();
}
