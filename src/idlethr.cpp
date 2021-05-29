#include "system.h"
#include "idlethr.h"
#include "pcb.h"

IdleThread::IdleThread(): Thread(256, 1) {}

void IdleThread::run() {
	while (1);
}

void IdleThread::start() {
	lock();
	myPCB->curState = PCB::READY;
	myPCB->createStack();
	unlock();
}
