#ifndef _THREAD_H_
#define _THREAD_H_

class PCB;

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time;
const Time defaultTimeSlice = 2;
typedef int ID;

class Thread {
public:
	void start();
	void waitToComplete();
	virtual ~Thread();
	ID getId();
	static ID getRunningId();
	static Thread * getThreadById(ID id);

protected:
	friend class System;
	friend class IdleThread;
	friend class PCB;

	Thread (StackSize stackSize=defaultStackSize, Time timeSlice=defaultTimeSlice);
	virtual void run() {}

private:
	PCB* myPCB;
};

void dispatch();
// void tick();

#endif
