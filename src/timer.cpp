#include "helper.h"

pInterrupt ttimer;
volatile int timePass = defaultTimeSlice;

void tick();

void inic() {
	intd;
	ttimer = getvect(8);
	setvect(8, timer);
	inte;
}

void restore() {
	intd;
	setvect(8, ttimer);
	inte;
}

void interrupt timer(...) {
	if (!call) {
		ttimer();
		KernelSem::decrease();
		tick();

		timePass -= timePass > 0;
	}

	if (call || (timePass == 0 && locks == 0 && PCB::running->PCBtimePass != 0)) {
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
		timePass = PCB::running->PCBtimePass;

	} else if (timePass == 0 && locks > 0) contextReady = true;
}
