#include "kernelse.h"
#include "system.h"
#include "pcb.h"
#include "schedule.h"

KernelSem::KernelSem (int init) {
	value = init;
	waitQueue = new Queue();
}

int KernelSem::wait(Time t) {
	lock();
	if(--value < 0){
		PCB *pcb = (PCB*)System::running;
		pcb->curState = PCB::BLOCKED;
		pcb->isSignaled = 0;
		waitQueue->put(pcb);
		if (t > 0) System::TQueue->put(pcb,t,this);
		System::dispatch();
		unlock();
		return pcb->isSignaled;
	}
	unlock();
}

void KernelSem::signal() {
	lock();
	if (value++ < 0 && waitQueue->notEmpty()){
		PCB *pcb = waitQueue->get();
			System::TQueue->removePCB(pcb);
			pcb->isSignaled = 1;
			pcb->curState = PCB::READY;
			Scheduler::put(pcb);
		}
	unlock();
}

KernelSem::~KernelSem() {
	PCB *pcb;
		while (waitQueue->notEmpty()){
			pcb = waitQueue->get();
			pcb->curState = PCB::READY;
			Scheduler::put(pcb);
		}
		delete waitQueue;
}

int KernelSem::val() const  {
	return value;
}
