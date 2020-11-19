/*
 * System.cpp
 *
 *  Created on: 23 Oct 2020
 *      Author: arion
 */

#include "System.h"
//#include "usart.h"
#include "Misc/WatchdogThread.h"
#include "Sensors/Barometer.h"
#include "Sensors/IMU.h"

void initCortexM4() {
	new WatchdogThread();
	new IMUThread(&hi2c1);
	new BarometerThread(&hi2c1);
}

void initCortexM7() {
	new WatchdogThread();
}
