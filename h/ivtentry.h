
#ifndef H_IVTENTRY_H_
#define H_IVTENTRY_H_
#include "system.h"
#include "kernelev.h"

typedef void interrupt(*pInterrupt)(...);

class IVTEntry{
public:
	static IVTEntry *ivTable[];
	unsigned ivtNum;
	KernelEv *myEvent;
	pInterrupt old;
	IVTEntry(unsigned ivtNum, pInterrupt New);
	~IVTEntry();
	void signal();
	void callOld();
};


#endif
