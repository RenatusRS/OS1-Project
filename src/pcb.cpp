#include "helper.h"

volatile PCB *PCB::running = nullptr;
volatile Vector<PCB *> PCB::threads;
volatile ID PCB::globalID = 0;
PCB* PCB::forkChild = nullptr;

PCB::PCB(StackSize stackSize, Time timeSlice, Thread *thread, void target()) : stackSize(stackSize), PCBtimeSlice(timeSlice), thread(thread) {
	PCBlocks = semaphorTime = 0;
	state = INITIALIZING;
	semaphorSignaled = false;
	parent = nullptr;

	stackSize /= sizeof(unsigned);

	lock;

	stack = new unsigned[stackSize];

	unlock;

	stack[stackSize - 1] = 0x200;
	stack[stackSize - 2] = FP_SEG(target);
	stack[stackSize - 3] = FP_OFF(target);
	stack[stackSize - 12] = 0;

	sp = bp = FP_OFF(stack + stackSize - 12);
	ss = FP_SEG(stack + stackSize - 12);

	lock;

	id = globalID++;
	threads.pushf(this);

	unlock;
}

PCB::PCB() : stack(nullptr), thread(nullptr), parent(nullptr) {
	PCBlocks = semaphorTime = stackSize = sp = ss = bp = 0;

	state = RUNNING;
	PCBtimeSlice = defaultTimeSlice;

	semaphorSignaled = false;

	lock;

	id = globalID++;
	threads.pushf(this);

	unlock;
}

PCB::~PCB() {
	lock;

	delete[] stack;

	for (--threads; threads.get() != this; threads++);
	threads.remove();

	unlock;
}

void PCB::start() {
	lock;
	state = READY;
	Scheduler::put(this);
	unlock;
}

void PCB::waitToComplete() {
	lock;

	if (PCB::running != this && state != TERMINATING && state != INITIALIZING) {
		PCB::running->state = SUSPENDED;
		waiting.pushb((PCB *) PCB::running);
		dispatch();
	}

	unlock;
}

void PCB::wrapper() {
	running->thread->run();
	PCB::exit();
}

void idling() {
	while (true);
}

PCB *PCB::idler() {
	static PCB idle(256, 1, nullptr, idling);
	return &idle;
}

Thread *PCB::getThreadById(ID id) {
	lock;
	for (--threads; threads.get() != nullptr && threads.get()->id != id; threads++);
	unlock;

	return threads.get() ? threads.get()->thread : nullptr;
}

void PCB::exit() {
	lock;
	running->state = TERMINATING;

	while (running->waiting.size) {
		PCB *releasePCB = running->waiting.popb();
		releasePCB->state = READY;
		Scheduler::put((PCB*) releasePCB);
	}

	if (running->parent) {

		for (--running->parent->children; running->parent->children.get() != running; running->parent->children++);
		running->parent->children.remove();

		if (running->parent->children.size == 0 && running->parent->state == CHILD_SUSPENDED) {
			running->parent->state = READY;
			Scheduler::put((PCB*) running->parent);
		}
	}

	for (--running->children; running->children.get() != nullptr; running->children++) ((PCB*) running->children.get())->parent = nullptr;

	dispatch();
}

void PCB::waitForForkChildren() {
	if (running->children.size > 0) {
		running->state = CHILD_SUSPENDED;
		dispatch();
	}
}

void interrupt PCB::fork() {
	memcpy(forkChild->stack, running->stack, running->stackSize);

	forkChild->bp = forkChild->sp = _SI = _BP - FP_OFF(running->stack) + FP_OFF(forkChild->stack);  // BP - StariStack = indeks gde se BP nalazi u starom stacku. Dodamo na to novi stack.

	_ES = forkChild->ss;            // ES = stack u detetu
	_BX = _BP;                      // BX = stari BP u roditelju
                                    // SS = stack u roditelju

	while (true) {
		asm mov ax, word ptr ss:bx; // AX = [vrednost] starog BP u roditelju

		if (_AX == 0) return;       // Ako je [vrednost] = 0 stigli smo do kraja

		_AX -= _BX;                 // AX = [vrednost] - stariBP = pomeraj izmedju dva bp

		_DX = _SI;                  // DX = SI = trenutni BP koji menjamo, u prvom prolazu smo ga postavili gore u novom stacku
		_DX += _AX;                 // DX += AX = trenutni BP koji menjamo + pomeraj izmedju dva bp

		asm	mov word ptr es:si, dx; // Upisujemo ispravljeni BP u dete

		_BX += _AX;                 // Pomeramo BX vrednost u roditelju na sledeci BP u roditelju
		_SI = _DX;                  // SI je sada DX nakon AX, tj sledeci BP u detetu
	}
}
