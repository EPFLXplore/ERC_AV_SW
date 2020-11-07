/*
 * WatchdogThread.h
 *
 *  Created on: 7 Nov 2020
 *      Author: arion
 */

#ifndef MISC_WATCHDOGTHREAD_H_
#define MISC_WATCHDOGTHREAD_H_

#include "../Thread.h"

class WatchdogThread : Thread {
public:
	WatchdogThread() : Thread("Watchdog") {}
	void init();
	void loop();
};

#endif /* MISC_WATCHDOGTHREAD_H_ */
