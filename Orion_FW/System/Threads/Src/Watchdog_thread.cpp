/*
 * Watchdog_thread.cpp
 *
 *  Created on: Aug 10, 2023
 *      Author: Vincent Nguyen
 */


#include "Watchdog_thread.h"
#include "Debug.h"
#include <string.h>
#include "iwdg.h"

#include "Telemetry.h"
#include "DataStructures.h"

void WatchdogThread::init() {
	LOG_INFO("Thread successfully created");
}

void WatchdogThread::loop() {
	HAL_IWDG_Refresh(watchdog_handle);
}
