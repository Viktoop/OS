#ifndef EVENT_H_
#define EVENT_H_

#include "ivtentry.h"

typedef unsigned char IVTNo;

class KernelEv;

#define PREPAREENTRY(ivtno,old) \
void interrupt routine##ivtno(...); \
IVTEntry ent##ivtno(ivtno,routine##ivtno); \
void interrupt routine##ivtno(...) { \
	if (old) ent##ivtno.callOld(); \
	ent##ivtno.signal(); \
}

class Event {
public:
	Event (IVTNo ivtNo);
	~Event ();
	void wait ();
protected:
	friend class KernelEv;
	void signal();
private:
	KernelEv* myImpl;
};



#endif
