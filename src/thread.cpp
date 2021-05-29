#include "system.h"
#include "pcb.h"
#include "thread.h"
#include "schedule.h"
#include <dos.h>
#include <stdlib.h>

class System;
class Scheduler;

Thread::Thread(StackSize stackSize, Time timeSlice) {
	lock();
	myPCB = new PCB(this, stackSize, (LongTime)timeSlice);
	unlock();
}

Thread::~Thread() {
	lock();
	waitToComplete();
	delete myPCB;
	unlock();
}

void Thread::start() {
	lock();
	myPCB->curState = PCB::READY;
	myPCB->createStack();
	Scheduler::put(myPCB);
	unlock();
}

void Thread::waitToComplete() {
	lock();
	if (myPCB->curState == PCB::OVER || myPCB->curState == PCB::NEW) { unlock(); return; }
	if (myPCB == (PCB*)System::running) { unlock(); return; }
	if (this == System::starting) { unlock(); return; }
	if (this == System::idle) { unlock(); return; }
	System::running->curState = PCB::BLOCKED;
	myPCB->waitQueue->put((PCB*)System::running);
	System::dispatch();
	unlock();
}

ID Thread::getId(){
		return myPCB->id;
}

ID Thread::getRunningId(){
	return System::running->id;
}

Thread* Thread::getThreadById(ID id){
	PCB *t = System::PCBList->getPCBById(id);
	if(t == 0) return 0;
	return t->myThread;
}
void dispatch() {
	lock();
	System::dispatch();
	unlock();
}

//void tick() {};
