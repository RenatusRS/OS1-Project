#ifndef KEREVENT_H
#define KEREVENT_H

#include "helper.h"

class KernelEv {
	IVTNo ivtNo;
	bool val;
	PCB *pcb;

public:
	KernelEv(IVTNo ivtNo);

	~KernelEv();

	void wait();

	void signal();
};

#endif
