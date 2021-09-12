#include "helper.h"

pInterrupt ttimer;
unsigned tsp;
unsigned tss;
unsigned tbp;
volatile int timeLeft = defaultTimeSlice;

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

void interrupt timer(...){
	if (!call) {
		(*ttimer)();
		KernelSem::decrease();
		tick();

		timeLeft -= timeLeft > 0;
	}

	if (call || (timeLeft == 0 && locks == 0 && PCB::running->timerPasses != 0)) {
		call = false;
		queuedContext = false;

		move tsp, sp;
		move tss, ss;
		move tbp, bp;

		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp = tbp;
		PCB::running->PCBlocks = locks;

		if (PCB::running->state == RUNNING) {
			PCB::running->state = READY;
			Scheduler::put((PCB*) PCB::running);
		}

		if ((PCB::running = Scheduler::get()) == nullptr) PCB::running = PCB::idler();
		else PCB::running->state = RUNNING;

		timeLeft = PCB::running->timerPasses;

		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;
		locks = PCB::running->PCBlocks;

		move sp, tsp;
		move ss, tss;
		move bp, tbp;

	} else if (timeLeft == 0 && locks > 0) queuedContext = true;
}
