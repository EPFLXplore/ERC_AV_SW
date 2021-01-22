/*
 * System.cpp
 *
 *  Created on: 23 Oct 2020
 *      Author: arion
 */

#include "System.h"

#include "Lang/Operators.h"

#include "i2c.h"
#include "iwdg.h"
#include "usart.h"

#include "Misc/WatchdogThread.h"
#include "Sensors/Barometer.h"
#include "Sensors/IMU.h"

#ifdef CORE_CM4
void initCortexM4() {
	static WatchdogThread watchdog(&hiwdg2);
	/*IMUThread imu(&hi2c1, &huart3);
	BarometerThread barometer(&hi2c1, &huart3);*/
}
#endif

#ifdef CORE_CM7
void initCortexM7() {
	static WatchdogThread watchdog(&hiwdg1);
}
#endif
