/*
 * Thread.cpp
 *
 *  Created on: 23 Oct 2020
 *      Author: AV Team 2020
 */

#include "Thread.h"
#include "iwdg.h"

#include "usart.h"
#define DEFAULT_STACK_SIZE (512)


void __task_run(const void* arg) {
	Thread* thread = (Thread*) arg;

	thread->init();

	while(true) {
		thread->loop();
	}
}

Thread::Thread(const char* name) : Thread(name, (osPriority) osPriorityNormal) {
	;
}

Thread::Thread(const char* name, osPriority priority) : Thread(name, priority, DEFAULT_STACK_SIZE) {
	;
}

Thread::Thread(const char* name, uint32_t stackSize) : Thread(name, (osPriority) osPriorityNormal, stackSize) {
	;
}

Thread::Thread(const char* name, osPriority priority, uint32_t stackSize) {
	osThreadDef(T, __task_run, priority, 0, stackSize);
	this->handle = osThreadCreate(osThread(T), this);
}
