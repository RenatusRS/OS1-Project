#include "helper.h"

IVTEntry* IVTEntry::table[256] = {nullptr};

IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt newRoutine) : ivtNo(ivtNo), kernelev(nullptr), oldRoutine(getvect(ivtNo))  {
    intd;
    setvect(ivtNo, newRoutine);
	table[ivtNo] = this;
	inte;
}

IVTEntry::~IVTEntry() {
    intd;
    setvect(ivtNo, oldRoutine);
    inte;
}

void IVTEntry::signal() {
	kernelev->signal();
}
