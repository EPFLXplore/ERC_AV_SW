/*
 * WatchdogThread.hpp
 *
 *  Created on: 7 Nov 2020
 *      Author: AV Team 2020
 */

#ifndef MISC_WATCHDOGTHREAD_H_
#define MISC_WATCHDOGTHREAD_H_

#include "../Thread.h"

class WatchdogThread : Thread {
public:
	WatchdogThread(IWDG_HandleTypeDef* hiwdg) : Thread("Watchdog"), watchdog_handle(hiwdg) {}
	void init();
	void loop();
private:
	IWDG_HandleTypeDef* watchdog_handle;
};

#endif /* MISC_WATCHDOGTHREAD_H_ */
