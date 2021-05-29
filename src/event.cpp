#include "event.h"
#include "kernelev.h"
#include "system.h"

Event::Event(IVTNo ivtNo) {
	lock();
		myImpl = new KernelEv(ivtNo);
	unlock();
}

void Event::wait() {
	lock();
		myImpl->wait();
	unlock();
}
void Event::signal() {
	lock();
		myImpl->signal();
	unlock();
}

Event::~Event() {
	lock();
		delete myImpl;
	unlock();
}



