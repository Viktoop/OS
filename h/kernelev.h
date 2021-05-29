#ifndef KERNELEV_H_
#define KERNELEV_H_

#include "thread.h"
#include "pcb.h"
#include "kernelse.h"

typedef unsigned char IVTNo;


class KernelEv {
	Thread *owner;
public:
	IVTNo ivtNo;
	Thread* thread;
	KernelSem* sem;

	KernelEv (IVTNo ivtNo);
	~KernelEv ();
	void wait ();
	void signal();

};


#endif
