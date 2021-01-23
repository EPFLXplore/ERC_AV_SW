/*
 * WatchdogThread.cpp
 *
 *  Created on: 7 Nov 2020
 *      Author: arion
 */


#include "WatchdogThread.h"

#include <string.h>
#include "iwdg.h"

#include "usart.h"

void WatchdogThread::init() {

}

void WatchdogThread::loop() {
	HAL_IWDG_Refresh(watchdog_handle);
	osDelay(100);
}
