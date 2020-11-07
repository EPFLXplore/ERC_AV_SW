/*
 * System.cpp
 *
 *  Created on: 23 Oct 2020
 *      Author: arion
 */

#include "System.h"

#include "Misc/WatchdogThread.h"

void initCortexM4() {
	new WatchdogThread();
}

void initCortexM7() {
	new WatchdogThread();
}
