#include "helper.h"

IVTEntry* IVTEntry::table[256] = {nullptr};

IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt newRt) : ivtNo(ivtNo), kernelev(nullptr), oldRt(getvect(ivtNo))  {
    intd;
    setvect(ivtNo, newRt);
	table[ivtNo] = this;
	inte;
}

IVTEntry::~IVTEntry() {
    intd;
    setvect(ivtNo, oldRt);
    inte;
}

void IVTEntry::signal() {
	kernelev->signal();
}
