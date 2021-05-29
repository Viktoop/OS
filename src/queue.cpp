#include <stdlib.h>
#include "queue.h"
#include "system.h"
#include "semaphor.h"
#include "kernelse.h"

Queue::Queue() {
	lock();
	first = last = 0;
	size = 0;
	unlock();
}

Queue::~Queue() {
	lock();
	Elem* temp;
	while( first != NULL )
	{
		temp = first;
		first = first->next;
		delete temp;
		size--;
	}
	unlock();
}



PCB* Queue::get() {
	if(first == 0) return 0;
	Elem* temp = first;
	first = first->next;
	if (first == 0) last = 0;
	PCB* tmp = temp->pcb;
	temp->pcb = 0;
	delete temp;
	size--;
	return tmp;
}

void Queue::put(PCB* p) {
	lock();
	Elem* niew = new Elem(p);
	if(first == 0){ first = niew;}
	else {last->next = niew;}
	last = niew;
	size++;
	unlock();
}

PCB* Queue::getPCBById(ID id){
	Elem* temp = first;
	while(temp){
		if(temp->pcb->id == id) { return temp->pcb; }
		temp = temp->next;
	}
	return 0;
}

void Queue::removePCB(PCB* p){
	Elem* current, *past;
		current = first;
		past = 0;
		while (current && current->pcb != p){
			past = current;
			current = current->next;
		}
		if (current){
		if (!past) first = current->next;
		else{
			past->next = current->next;
			current->next = 0;
		}
		if (current == last) last = past;
		delete(current);
		}
}
////////////////////////////////////////////////////// TIME QUEUE /////////////////////////////////////////

TimeQueue::TimeQueue(){
	lock();
	first = 0;
	unlock();
}
void TimeQueue::put(PCB *p,Time t, KernelSem *sem){
	lock();
	Elem *n = new Elem(p,t,sem);
	Elem *temp = first;
	Elem *past = 0;
		while (temp && temp->timeToWait < n->timeToWait){
		n->timeToWait = n->timeToWait - temp->timeToWait;
		past = temp;
		temp = temp->next;
	}
	if (temp) temp->timeToWait = temp->timeToWait - n->timeToWait;
	n->next = temp;
	if (past) past->next = n;
	else
		first = n;
	unlock();
}
void TimeQueue::removePCB(PCB *p){
	lock();
	Elem *temp = first;
	Elem *past = 0;
		while (temp && temp->pcb != p){
			past = temp;
			temp = temp->next;
		}
		if (temp){
			if (past) past->next = temp->next;
			else first = temp->next;
			if (temp->next)
				temp->next->timeToWait = temp->next->timeToWait + temp->timeToWait;
			delete temp;
		}
		unlock();
}
int TimeQueue::isEmpty(){
	if(first) return 1;
	return 0;
}
PCB* TimeQueue::get(){
	lock();
	PCB* p;
	Elem *current = first;
	current->semaphore->waitQueue->removePCB(current->pcb);
	current->pcb->curState = PCB::READY;
	current->semaphore->value++;
	current->pcb->isSignaled = 0;
	p = current->pcb;
	first = first->next;
	delete current;
	unlock();
	return p;
}
TimeQueue::~TimeQueue(){

}
