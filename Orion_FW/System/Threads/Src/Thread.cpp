/*
 * Thread.cpp
 *
 *  Created on: 2021
 *      Author: AV Team 2021
 */

#include "Operators.h"
#include "Debug.h"

#include "usart.h"

#include <stdarg.h>
#include <string.h>
#include <Thread.h>

#define DEFAULT_STACK_SIZE (2048) // Danger zone: changing the stack size might create very nasty bugs

static char buffer[128];

void __task_run(void* arg) {
	Thread* thread = (Thread*) arg;

	osDelay(100 / portTICK_PERIOD_MS);

	thread->init();

	while(thread->isRunning()) {
		thread->loop();
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

	sprintf(buffer, "[%s] ", name);
	vsprintf(buffer + strlen(buffer), format, args);
	strcat(buffer, "\r\n");

	// Clear the line to keep the console clean
	console.print("\033[2K");
	console.print(buffer);

	va_end(args);
}

void Thread::LOG_INFO(const char* format, ...) {
	va_list args;
	va_start(args, format);
	va_end(args);

	sprintf(buffer, "[%s] ", name);
	vsprintf(buffer + strlen(buffer), format, args);
	strcat(buffer, "\r\n");

	// Clear the line to keep the console clean
	console.print("\033[2K");

    const char *colorStart = "\x1B[33m"; // Green text color
    const char *colorEnd = "\x1B[39;49m";   // Reset text color

    console.print(colorStart);
	console.print(buffer);
    console.print(colorEnd);

	va_end(args);
}

void Thread::LOG_SUCCESS(const char* format, ...) {
	va_list args;
	va_start(args, format);
	va_end(args);

	sprintf(buffer, "[%s] ", name);
	vsprintf(buffer + strlen(buffer), format, args);
	strcat(buffer, "\r\n");

	// Clear the line to keep the console clean
	console.print("\033[2K");

    const char *colorStart = "\x1B[32m"; // Green text color
    const char *colorEnd = "\x1B[39;49m";   // Reset text color

    console.print(colorStart);
	console.print(buffer);
    console.print(colorEnd);

	va_end(args);
}

void Thread::LOG_ERROR(const char* format, ...) {
	va_list args;
	va_start(args, format);
	va_end(args);

	sprintf(buffer, "[%s] ", name);
	vsprintf(buffer + strlen(buffer), format, args);
	strcat(buffer, "\r\n");

	// Clear the line to keep the console clean
	console.print("\033[2K");

    const char *colorStart = "\x1B[31m"; // Green text color
    const char *colorEnd = "\x1B[39;49m";   // Reset text color

    console.print(colorStart);
	console.print(buffer);
    console.print(colorEnd);

	va_end(args);
}


void Thread::setTickDelay(uint32_t ms) {
	this->delay = ms;
}

uint32_t Thread::getTickDelay() {
	return this->delay;
}
