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

uint8_t i = 0;

void WatchdogThread::loop() {
	HAL_IWDG_Refresh(watchdog_handle);
	#ifdef CORE_CM7
	i++;
	HAL_UART_Transmit(&huart3, &i, 1, 1000);
#endif
	osDelay(100);
}
