#ifndef _PCB_H_
#define _PCB_H_

#include "thread.h"
#include "queue.h"

typedef unsigned long LongTime;
class Thread;
class Queue;
static unsigned lastID = 0;

class PCB {
public:
	unsigned isSignaled;
	unsigned int sp, ss;
	static const int NEW, READY, BLOCKED, OVER;
	volatile int curState;
	volatile LongTime timePassedCounter;
	unsigned* pcbSP;
	Thread* myThread;
	ID id;
	Time pcbTimeSlice;
	StackSize pcbStackSize;
	Queue* waitQueue;


	PCB(Thread* myT, StackSize stackSize, LongTime timeSlice);
	void createStack();
	~PCB();
};

#endif
