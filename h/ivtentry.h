#ifndef IVTENTRY
#define IVTENTRY

#include "helper.h"

#define PREPAREENTRY(ivtno, callold)                            \
        void interrupt newIntr##ivtno(...);                     \
        IVTEntry newEntry##ivtno(ivtno, newIntr##ivtno);        \
        void interrupt newIntr##ivtno(...) {                    \
            newEntry##ivtno.signal();                           \
            if (callold == 1) newEntry##ivtno.routine();        \
}

class IVTEntry {
public:
	static IVTEntry *table[256];

	IVTNo ivtno;
	pInterrupt routine;
	KernelEv *kernelev;

	IVTEntry(IVTNo ivtno, pInterrupt routineNew);

	~IVTEntry();

	void signal();
};

#endif
