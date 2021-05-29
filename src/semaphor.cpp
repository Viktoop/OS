#include <stdlib.h>
#include "semaphor.h"
#include "kernelse.h"
#include "system.h"
#include "schedule.h"

int semPreempt = 0;

Semaphore::Semaphore(int init) {
	lock();
	myImpl = new KernelSem(init);
	unlock();
}

Semaphore::~Semaphore() {
	lock();
	delete myImpl;
	unlock();
}

int Semaphore::wait(Time t) {
	lock();
	int res = myImpl->wait(t);
	unlock();
	return res;
}

void Semaphore::signal() {
	lock();
	myImpl->signal();
	unlock();
}

int Semaphore::val() const {
	return myImpl->val();
}

