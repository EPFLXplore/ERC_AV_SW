/*
 * Thread.cpp
 *
 *  Created on: 23 Oct 2020
 *      Author: arion
 */

#include "Thread.hpp"

#define DEFAULT_STACK_SIZE (512 * 4)


void __task_run(void* arg) {
	Thread* thread = (Thread*) arg;

	thread->init();

	while(true) {
		thread->loop();
	}
}

Thread::Thread(const char* name) : Thread(name, (osPriority_t) osPriorityNormal) {
	;
}

Thread::Thread(const char* name, osPriority_t priority) : Thread(name, priority, DEFAULT_STACK_SIZE) {
	;
}

Thread::Thread(const char* name, uint32_t stackSize) : Thread(name, (osPriority_t) osPriorityNormal, stackSize) {
	;
}

Thread::Thread(const char* name, osPriority_t priority, uint32_t stackSize) {
	osThreadAttr_t attributes;

	attributes.name = name;
	attributes.priority = priority;
	attributes.stack_size = stackSize;

	this->handle = osThreadNew(__task_run, this, &attributes);
}
