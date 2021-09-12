#include "helper.h"

KernelEv::KernelEv(IVTNo ivtno) : ivtNo(ivtno), pcb((PCB *) PCB::running), val(false) {
	IVTEntry::table[ivtno]->kernelev = this;
}

KernelEv::~KernelEv() {
	if (pcb->state == SUSPENDED) {
		pcb->state = READY;
		Scheduler::put(pcb);
	}
}

void KernelEv::wait() {
	if (pcb == PCB::running) {
		val = false;
		pcb->state = SUSPENDED;
		dispatch();
	}
}

void KernelEv::signal() {
	lock;

	if (val == false) {
		val = true;
		pcb->state = READY;
		Scheduler::put(pcb);
		dispatch();
	}

	unlock;

}
