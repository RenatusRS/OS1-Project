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

void Thread::waitToComplete() {
	myPCB->waitToComplete();
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

void dispatch() {
	intd;
	call = true;
	timer();
	inte;
}

Thread *Thread::clone() const {
	return new Thread(myPCB->stackSize, myPCB->PCBtimePass);
}

ID Thread::fork(){
	lock;

	Thread* child = PCB::running->thread->clone();
	if (!child || !child->myPCB || !child->myPCB->stack) {
		delete child;
		return -1;
	}

	PCB::forkChild = child->myPCB;

	PCB::fork();

	if (child->myPCB->id == PCB::running->id) return 0;

	PCB::running->children++;
	child->myPCB->parent = (PCB*) PCB::running;
	child->start();

	unlock;

	return child->myPCB->id;
}

void Thread::waitForForkChildren() {
	PCB::waitForForkChildren();
}

void Thread::exit() {
	PCB::exit();
}

