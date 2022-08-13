/*
 * WatchdogThread.h
 *
 *  Created on: Aug 10, 2022
 *      Author: YassineBakkali
 */

#ifndef MISC_WATCHDOGTHREAD_H_
#define MISC_WATCHDOGTHREAD_H_

#include "WatchdogThread.h"

#include <string.h>
#include "iwdg.h"

#include "Debug/Debug.h"
#include "usart.h"

void WatchdogThread::init() {

}

void WatchdogThread::loop() {
	HAL_IWDG_Refresh(watchdog_handle);
	osDelay(20 / portTICK_PERIOD_MS);
}

#endif /* MISC_WATCHDOGTHREAD_H_ */
