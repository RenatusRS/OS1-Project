#ifndef PCB_H
#define PCB_H

#include "helper.h"

enum State {
	INITIALIZING, READY, RUNNING, SUSPENDED, TERMINATING, IDLE
};

class PCB {
private:
	volatile static ID globalID;

public:
	volatile static PCB *running;
	volatile static Vector<PCB *> threads;

	Vector<PCB *> waitingForMe;

	unsigned *stack;
	unsigned sp;
	unsigned ss;
	unsigned bp;

	ID id;
	Thread *thread;
	int PCBlocks;
	int timerPasses;
	State state;

	bool unblTime;
	Time semaphorTime;
	Time semaphorLeft;

	PCB();

	PCB(StackSize stackSize, Time timeSlice, Thread *thread, void (*targer)() = PCB::worker, State state = INITIALIZING);

	~PCB();

	void waitToComplete();

	void start();

	void unblock();

	void block();

	static Thread *getThreadById(ID id);

	static void worker();

	static PCB *idler();
};

#endif
