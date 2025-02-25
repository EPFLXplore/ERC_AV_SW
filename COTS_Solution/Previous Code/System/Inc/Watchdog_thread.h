/*
 * Watchdog_thread.h
 *
 *  Created on: Aug 16, 2023
 *      Author: vince
 */

#ifndef THREADS_INC_WATCHDOG_THREAD_H_
#define THREADS_INC_WATCHDOG_THREAD_H_


#include "Thread.h"

class WatchdogThread : public Thread {
public:
	WatchdogThread(IWDG_HandleTypeDef* hiwdg) : Thread("Watchdog", osPriorityRealtime), watchdog_handle(hiwdg) {}
	void init();
	void loop();
private:
	IWDG_HandleTypeDef* watchdog_handle;
};


#endif /* THREADS_INC_WATCHDOG_THREAD_H_ */
