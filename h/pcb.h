#ifndef _pcb_h_
#define _pcb_h_

#include "helper.h"

enum State {
	INITIALIZING, READY, RUNNING, SUSPENDED, TERMINATING, SUSPENDED_EVENT, CHILD_SUSPENDED
};

class PCB {
private:
	volatile static ID globalID;

public:
	volatile static PCB *running;
	volatile static Vector<PCB*> threads;

	Vector<PCB*> waiting;
	Vector<PCB*> children;

	unsigned *stack;
	unsigned sp;
	unsigned ss;
	unsigned bp;

	ID id;
	Thread *thread;
	int PCBlocks;
	int PCBtimeSlice;
	State state;

	bool semaphorSignaled;
	Time semaphorTime;

	PCB();

	PCB(StackSize stackSize, Time timeSlice, Thread *thread, void target() = PCB::wrapper);

	~PCB();

	void start();

	void waitToComplete();

	static Thread *getThreadById(ID id);

	static void wrapper();

	static PCB *idler();

	// Fork

	StackSize stackSize;
	PCB* parent;
	static PCB* forkChild;

	static void exit();

	static void waitForForkChildren();

	static void interrupt fork();
};

#endif
