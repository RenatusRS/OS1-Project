#include "helper.h"

IVTEntry *IVTEntry::table[256];

IVTEntry::IVTEntry(IVTNo ivtno, pInterrupt routineNew) : ivtno(ivtno), kernelev(nullptr), routine(getvect(ivtno)) {
	intd;
	setvect(ivtno, routineNew);
	table[ivtno] = this;
	inte;
}

IVTEntry::~IVTEntry() {
	intd;
	setvect(ivtno, routine);
	inte;
}

void IVTEntry::signal() {
	kernelev->signal();
}
