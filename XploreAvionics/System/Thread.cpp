/*
 * Thread.cpp
 *
 *  Created on: 23 Oct 2020
 *      Author: arion
 */

#include "Thread.h"

#define DEFAULT_STACK_SIZE (512 * 4)


static void __task_run(Thread* thread) {
	thread->init();

	while(true) {
		thread->loop();
	}
}

Thread::Thread(const char* name) {
	Thread::Thread(name, (osPriority_t) osPriorityNormal);
}

Thread::Thread(const char* name, osPriority_t priority) {
	osThreadAttr_t attributes = { .name = name, .priority = priority, .stack_size = DEFAULT_STACK_SIZE };
	Thread::Thread(attributes);
}

Thread::Thread(osThreadAttr_t attributes) {
	this->handle = osThreadNew(__task_run, this, &attributes);
}
