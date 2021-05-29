#include <iostream.h>
#include <dos.h>
#include <stdlib.h>
#include "ivtentry.h"
#include "system.h"

IVTEntry * IVTEntry::ivTable[256];

IVTEntry::IVTEntry(unsigned ivtNum, pInterrupt New){
	lock();
	ivTable[ivtNum] = this;
	this->ivtNum = ivtNum;
	this->myEvent = NULL;

#ifndef BCC_BLOCK_IGNORE
	old = getvect(this->ivtNum);
	setvect(this->ivtNum,New);
#endif
unlock();
}

IVTEntry::~IVTEntry(){
	lock();
#ifndef BCC_BLOCK_IGNORE
	setvect(this->ivtNum,old);
#endif
	unlock();
}

void IVTEntry::callOld(){
	if(old)
		(*old)();
}

void IVTEntry::signal() {
	lock();
	if (myEvent)
		myEvent->signal();
	unlock();
}

