#include "helper.h"

Thread::Thread(StackSize stackSize, Time timeSlice) {
	lock;
	myPCB = new PCB(stackSize, timeSlice, this);
	unlock;
}

Thread::~Thread() {
	lock;
	delete myPCB;
	unlock;
}

void Thread::start() {
	myPCB->start();
}

ID Thread::getId() {
	return myPCB->id;
}

ID Thread::getRunningId() {
	return PCB::running->id;
}

Thread *Thread::getThreadById(ID id) {
	return PCB::getThreadById(id);
}

void Thread::waitToComplete() {
	myPCB->waitToComplete();
}

void dispatch() {
	intd;
	call = true;
	timer();
	inte;
}

// Thread *Thread::clone() const {}

// ID Thread::fork() { return 0; }

// void Thread::waitForForkChildren() {}

// void Thread::exit() { }

