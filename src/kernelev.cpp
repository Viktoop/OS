#include "system.h"
#include "kernelev.h"
#include "ivtentry.h"

KernelEv::KernelEv (IVTNo ivt) {
	if(IVTEntry::ivTable[ivt]) {
		this->owner=System::running->myThread;
		this->ivtNo=ivt;
		this->sem = new KernelSem(0);
		IVTEntry::ivTable[ivtNo]->myEvent = this;
	}
}

KernelEv::~KernelEv () {
	IVTEntry::ivTable[ivtNo]->myEvent = 0;
	delete sem;
}

void KernelEv::wait () {
	lock();
	if (System::running->myThread!=owner) return;
	sem->wait(0);
	unlock();
}

void KernelEv::signal() {
	if (sem->value==1) return;
	sem->signal();
}
