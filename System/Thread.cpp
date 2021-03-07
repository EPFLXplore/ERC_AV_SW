/*
 * Thread.cpp
 *
 *  Created on: 23 Oct 2020
 *      Author: AV Team 2020
 */

#include "Thread.h"

#include "Debug/Debug.h"

#include "iwdg.h"
#include "usart.h"

#include <stdarg.h>
#include <string.h>

#define DEFAULT_STACK_SIZE (512)

static char buffer[256];

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
	this->name = name;
}

void Thread::println(const char* format, ...) {
	va_list args;
	va_start(args, format);

	sprintf(buffer, "[%s] ", name);
	vsprintf(buffer + strlen(buffer), format, args);
	strcat(buffer, "\r\n");

	console.print(buffer);

	va_end(args);
}