#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "pcb.h"

class PCB;
class KernelSem;

class Queue {
public:
	struct Elem {
			PCB* pcb;
			Elem* next;
			Elem(PCB* p){
				pcb = p;
				next = 0;
			}
		};

	Elem* first,*last;
	int size;
	int notEmpty(){
		if (first) return 1;
		return 0;
	}
	void put(PCB* pcb);
	PCB* get();
	void removePCB(PCB* p);
	PCB *getPCBById(ID id);


	Queue();
	~Queue();

};
///////////////////////////////////////////////////////// TIME QUEUE /////////////////////////////////////

class TimeQueue {
public:
	friend class System;
	struct Elem {
			PCB* pcb;
			KernelSem* semaphore;
			Time timeToWait;
			Elem* next;
			Elem(PCB* p, Time time, KernelSem* sem, Elem* n = 0){
				this->pcb = p;
				this->timeToWait = time;
				this->semaphore = sem;
				this->next = n;
			}
	};

	Elem* first;

	TimeQueue();
	void put(PCB* p,Time t, KernelSem* sem);
	void removePCB(PCB* p);
	int isEmpty();
	PCB* get();
	~TimeQueue();

};

#endif
