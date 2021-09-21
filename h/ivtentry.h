#ifndef _ivtentry_h_
#define _ivtentry_h_

#include "helper.h"

#define PREPAREENTRY(num, callOld)                    \
		extern IVTEntry ivtEntry##num;                \
		void interrupt intrEv##num(...) {             \
			ivtEntry##num.signal();                   \
			if (callOld) ivtEntry##num.oldRt();       \
		}                                             \
		IVTEntry ivtEntry##num(num, intrEv##num);     \

class IVTEntry {
public:
    static IVTEntry *table[256];

    IVTNo ivtNo;
    KernelEv *kernelev;
    pInterrupt oldRt;

    IVTEntry(IVTNo ivtNo, pInterrupt newRt);

    ~IVTEntry();

    void signal();
};
#endif
