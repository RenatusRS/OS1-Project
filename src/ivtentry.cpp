#include "helper.h"

IVTEntry* IVTEntry::table[256] = {nullptr};

IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt newRt) : kernelev(nullptr), oldRt(getvect(ivtNo)) {
    intd;
    setvect(ivtNo, newRt);
	table[ivtNo] = this;
	inte;
}

void IVTEntry::signal() {
	kernelev->signal();
}
