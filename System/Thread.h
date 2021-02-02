/*
 * Thread.hpp
 *
 *  Created on: 23 Oct 2020
 *      Author: AV Team 2020
 */

#ifndef THREAD_H_
#define THREAD_H_

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "stm32h7xx_hal.h"

class Thread {
public:
	Thread(const char* name);
	Thread(const char* name, osPriority priority);
	Thread(const char* name, uint32_t stackSize);
	Thread(const char* name, osPriority priority, uint32_t stackSize);
	~Thread() {};
	virtual void init() = 0;
	virtual void loop() = 0;

protected:
	void println(const char* format, ...);

private:
	osThreadId handle;
	const char* name;
	void task();
};


#endif /* THREAD_H_ */
