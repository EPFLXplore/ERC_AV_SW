/*
 * WatchdogThread.cpp
 *
 *  Created on: 7 Nov 2020
 *      Author: arion
 */


#include "WatchdogThread.h"

#include "iwdg.h"

#ifdef CORE_CM7
IWDG_HandleTypeDef* watchdog_handle = &hiwdg1;
#elif defined CORE_CM4
IWDG_HandleTypeDef* watchdog_handle = &hiwdg2;
#endif


void WatchdogThread::init() {

}

void WatchdogThread::loop() {
	HAL_IWDG_Refresh(watchdog_handle);
	osDelay(100);
}
