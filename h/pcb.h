#ifndef _pcb_h_
#define _pcb_h_

#include "helper.h"

enum State {
	INITIALIZING, READY, RUNNING, SUSPENDED, TERMINATING, CHILD_SUSPENDED
};

class PCB {
private:
	volatile static ID globalID;

public:
	volatile static PCB *running;
	volatile static Vector<PCB *> threads;

	Vector<PCB*> waitingForMe;

	unsigned *stack;
	unsigned sp;
	unsigned ss;
	unsigned bp;

	ID id;
	Thread *thread;
	int PCBlocks;
	int timerPasses;
	State state;

	bool semaphorSignaled;
	Time semaphorTime;
	Time semaphorLeft;

	PCB();

	PCB(StackSize stackSize, Time timeSlice, Thread *thread, void (*target)() = PCB::worker);

	~PCB();

	void waitToComplete();

	void start();

	static Thread *getThreadById(ID id);

	static void worker();

	static PCB *idler();

	// Fork

	StackSize stackSize;
	PCB* parent;
	int children;
	static PCB* forkChild;

	static void exit();

	static void waitForForkChildren();

	static void interrupt fork();
};

#endif
