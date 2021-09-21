#include "helper.h"

KernelEv::KernelEv(IVTNo ivtNo) : ivtNo(ivtNo), pcb((PCB*) PCB::running), blocked(false) {
	IVTEntry::table[ivtNo]->kernelev = this;
}

KernelEv::~KernelEv() {
    intd;
    setvect(ivtNo, IVTEntry::table[ivtNo]->oldRt);
    inte;
}

void KernelEv::wait() {
	if (pcb == PCB::running) {
		blocked = true;
		pcb->state = SUSPENDED;
		dispatch();
	}
}

void KernelEv::signal() {
	if (blocked) {
    	blocked = false;
    	pcb->state = READY;
    	Scheduler::put(pcb);
    }

	dispatch();
}
