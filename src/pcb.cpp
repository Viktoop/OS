#include "pcb.h"
#include "system.h"
#include <stdlib.h>
#include <dos.h>

const int PCB::NEW = 0;
const int PCB::READY = 1;
const int PCB::BLOCKED = 2;
const int PCB::OVER = 3;

PCB::PCB(Thread* thread, StackSize stackSize, LongTime timeSlice) {

	lock();
	curState = NEW;
	pcbStackSize = stackSize;
	myThread = thread;
	pcbSP = NULL;
	id = lastID++;
	ss = 0;
	sp = 0;
	pcbTimeSlice = timeSlice;
	timePassedCounter = 0.;
	waitQueue = new Queue();
	isSignaled = 0;
	unlock();
}

PCB::~PCB() {
	lock();
	delete waitQueue;
	delete pcbSP;
	unlock();
}

void PCB::createStack() {
  	lock();
	this->pcbStackSize /= sizeof(unsigned);
	this->pcbSP = new unsigned[pcbStackSize];

	pcbSP[pcbStackSize-1] = 0x200;
	pcbSP[pcbStackSize-2] = FP_SEG(&(System::wrapper));
	pcbSP[pcbStackSize-3] = FP_OFF(&(System::wrapper));

	this->sp = FP_OFF(pcbSP + pcbStackSize - 12);
	this->ss = FP_SEG(pcbSP + pcbStackSize - 12);
	unlock();
}
