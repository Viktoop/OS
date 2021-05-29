#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "pcb.h"
#include "thread.h"
#include "idlethr.h"

#define lock() {asm{pushf; cli;}}
#define unlock() {asm{popf}}

extern void tick();
typedef void (interrupt *InterruptRoutine)(...);

class Thread;
class PCB;
class KernelSem;
class KernelEv;
class Queue;

class System {
public:
	static Queue* PCBList;
	static TimeQueue* TQueue;
	static Thread* starting;
	static IdleThread* idle;
	static volatile PCB* running;
	static volatile char dispatched;

	static void load();
	static void unload();
	static void dispatch();
	static void interrupt (*oldRoutine)(...);
	static void interrupt timerIR(...);
	static void wrapper();
	static void refreshTQueue();
};

#endif
