/*
 * System.cpp
 *
 *  Created on: 23 Oct 2020
 *      Author: arion
 */

#include "System.h"

#include "Misc/WatchdogThread.h"

void initCortexM4() {
	WatchdogThread* watchdog = (WatchdogThread*) pvPortMalloc(sizeof(WatchdogThread));
	watchdog->WatchdogThread();
}

void initCortexM7() {

}
