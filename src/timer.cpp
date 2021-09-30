#include "helper.h"

pInterrupt oldTimer;
volatile int timeSlice = defaultTimeSlice;

void tick();

void inic() {
	intd;
	oldTimer = getvect(8);
	setvect(8, timer);
	inte;
}

void restore() {
	intd;
	setvect(8, oldTimer);
	inte;
}

void interrupt timer(...) {
	if (!call) {
		oldTimer();
		KernelSem::decrease();
		tick();

		timeSlice -= timeSlice > 0;
	}

	if (call || (timeSlice == 0 && locks == 0 && PCB::running->PCBtimeSlice != 0)) {
		call = contextReady = false;

		PCB::running->sp = _SP;
		PCB::running->ss = _SS;
		PCB::running->bp = _BP;
		PCB::running->PCBlocks = locks;

		if (PCB::running->state == RUNNING) {
			PCB::running->state = READY;
			Scheduler::put((PCB*) PCB::running);
		}

		if ((PCB::running = Scheduler::get()) != nullptr) PCB::running->state = RUNNING;
		else PCB::running = PCB::idler();

		_SP = PCB::running->sp;
		_SS = PCB::running->ss;
		_BP = PCB::running->bp;
		locks = PCB::running->PCBlocks;
		timeSlice = PCB::running->PCBtimeSlice;

	} else if (timeSlice == 0 && locks > 0) contextReady = true;
}
