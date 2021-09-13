#ifndef _ivtentry_h_
#define _ivtentry_h_

#include "helper.h"

#define PREPAREENTRY(ivtNum, callOld)								\
		extern IVTEntry ivtEntry##ivtNum;							\
		void interrupt interruptEvent##ivtNum(...) {				\
			ivtEntry##ivtNum.signal();								\
			if (callOld) ivtEntry##ivtNum.oldRoutine();				\
			dispatch();												\
		}															\
		IVTEntry ivtEntry##ivtNum(ivtNum, interruptEvent##ivtNum);	\

class IVTEntry {
public:
    static IVTEntry *table[256];

    IVTNo ivtNo;
    KernelEv *kernelev;
    pInterrupt oldRoutine;

    IVTEntry(IVTNo num, pInterrupt newRoutine);
    ~IVTEntry();

    void signal();
};
#endif
