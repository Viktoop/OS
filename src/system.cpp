#include "system.h"
#include "schedule.h"
#include <dos.h>
#include <iostream.h>
#include <stdlib.h>

volatile char System::dispatched = 0;
void interrupt (*System::oldRoutine)(...) = NULL;
volatile PCB* System::running = NULL;
Thread* System::starting = NULL;
IdleThread* System::idle = NULL;
Queue* System::PCBList = NULL;
TimeQueue* System::TQueue = NULL;

void System::load() {
	lock();
	oldRoutine = getvect(0x08);
	setvect(0x08, timerIR);
	starting = new Thread(0x10000, 1);
	TQueue = new TimeQueue();
	starting->myPCB->curState = PCB::READY;
	running = (volatile PCB*)starting->myPCB;
	idle = new IdleThread();
	idle->start();
	unlock();
}

void System::unload() {
	lock();
	setvect(0x08, oldRoutine);	
	delete starting;
	delete idle;
	unlock();
}

void System::dispatch() {
	lock();
	dispatched = 1;
	timerIR();
	dispatched = 0;
	unlock();
}	
void System::refreshTQueue(){
	lock();
	if (TQueue && TQueue->first){
		if(TQueue->first->timeToWait > 0)
			TQueue->first->timeToWait--;
		while (TQueue->first && TQueue->first->timeToWait <= 0){ // promenili == na <=

			Scheduler::put(TQueue->get());
		}
	}
	unlock();
}
void interrupt System::timerIR(...) {
	static volatile unsigned int tsp, tss;
	static volatile PCB *newThread;

	if(!dispatched ) {
		refreshTQueue();
		tick(); 
		(*oldRoutine)(); 
	} 
	asm { cli };
	if (!dispatched && running->pcbTimeSlice != 0) 
		running->timePassedCounter += 1;
	if (!dispatched && (running->timePassedCounter < running->pcbTimeSlice || running->pcbTimeSlice == 0) ) return;
	dispatched = 0; 

	if( running->curState == PCB::READY && running != idle->myPCB )
		Scheduler::put((PCB*)running); 


		newThread = Scheduler::get();
		if (newThread == NULL) newThread = idle->myPCB;
		
		asm { 
			mov tsp, sp
			mov tss, ss
		}
		
		running->sp = tsp;
		running->ss = tss;
		running = newThread;
		tsp = running->sp;
		tss = running->ss;
		
		asm {
			mov sp, tsp
			mov ss, tss
		}
		
		running->timePassedCounter = 0.;

}

void System::wrapper() {
	running->myThread->run();
	lock();
	running->curState = PCB::OVER;
	PCB* temp;
	while( running->waitQueue->size > 0 )
	{
		temp = running->waitQueue->get();
		temp->curState = PCB::READY;
		Scheduler::put(temp); 
	}
	dispatch();
}
