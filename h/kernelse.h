#ifndef _KERNELSE_H_
#define _KERNELSE_H_

#include "queue.h"

class KernelSem {
public:
	friend class Semaphore;

	int wait(Time t);
	void signal();

	int val() const;

	KernelSem(int init=1);
	~KernelSem();

	int value;
	Queue *waitQueue;
};

#endif
