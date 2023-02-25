/*
 * Thread.cpp
 *
 *  Created on: 2021
 *      Author: AV Team 2021
 */

#include "Thread.h"
#include "Lang/Operators.h"

#include "usart.h"

#include <stdarg.h>
#include <string.h>

#define DEFAULT_STACK_SIZE (2048) // Danger zone: changing the stack size might create very nasty bugs

static char buffer[128];

void __task_run(void* arg) {
	Thread* thread = (Thread*) arg;

	osDelay(100 / portTICK_PERIOD_MS);

	thread->init();

	while(thread->isRunning()) {
		thread->loop();
//		osDelay(1 / portTICK_PERIOD_MS);
		osDelay(thread->getTickDelay() / portTICK_PERIOD_MS);
	}

	delete thread;

	vTaskDelete(nullptr);

	while(true) {
		osDelay(1000 / portTICK_PERIOD_MS);
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

Thread::Thread(const char* name, osPriority priority, uint32_t stackSize): delay(10) {
	const osThreadAttr_t thread_attributes = {
	  .name = (char*) name,
	  .stack_size = stackSize,
	  .priority = (osPriority_t) priority,
	};
	this->handle = (osThreadId) osThreadNew(&__task_run, this, &thread_attributes);
	this->name = name;
}

osThreadId Thread::getHandle() {
	return handle;
}

void Thread::terminate() {
	this->running = false;
}

void Thread::println(const char* format, ...) {
	va_list args;
	va_start(args, format);
	va_end(args);
}

void Thread::setTickDelay(uint32_t ms) {
	this->delay = ms;
}

uint32_t Thread::getTickDelay() {
	return this->delay;
}
